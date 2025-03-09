import can
import serial
import time
import threading
import datetime
import os
import argparse

# --- Configuration (Adjust as needed) ---
CAN_INTERFACE = 'can0'
SERIAL_PORT = '/dev/ttyUSB0'
BAUD_RATE = 38400  # Default baud rate for canbox communication
LOG_DIR = "can_logs"
# You may need to change this, depending on your distro.
CAN_INTERFACE_TYPE = 'socketcan'

# --- Global Variables ---
can_bus = None  # Global variable to hold the CAN bus object
serial_port = None # Global variable to hold the serial port object
running = True  # Global flag to control the logging threads
log_file = None # file handler

# --- Helper Functions ---

def setup_can(interface=CAN_INTERFACE, interface_type=CAN_INTERFACE_TYPE, bitrate=125000):
    """Sets up the CAN interface."""
    global can_bus

    try:
        # This part is specific to linux
        os.system(f'sudo slcan_attach -f -s6 -o  {interface}')
        os.system(f'sudo slcand -S 1000000 "${interface}" can0')
        os.system(f'sudo ip link set {interface} up')
        can_bus = can.interface.Bus(channel=interface, bustype=interface_type)

        print(f"CAN interface {interface} set up.")
        return can_bus # return Can Bus object.
    except Exception as e:
        print(f"Error setting up CAN interface: {e}")
        return None

def setup_serial(port=SERIAL_PORT, baudrate=BAUD_RATE):
    """Sets up the serial port."""
    global serial_port
    try:
        serial_port = serial.Serial(port, baudrate, timeout=1)
        print(f"Serial port {port} set up.")
        return serial_port
    except Exception as e:
        print(f"Error setting up serial port: {e}")
        return None


def can_logger(can_bus, log_file):
    """Logs CAN bus messages to the specified file."""
    global running
    print("CAN logger started.")
    while running:
        try:
            message = can_bus.recv(0.1)  # Timeout of 0.1 seconds
            if message:
                log_file.write(f"CAN,{message.timestamp:.6f},{message.arbitration_id:X},{message.dlc},")
                log_file.write(",".join(f"{byte:02X}" for byte in message.data))
                log_file.write("\n")

        except can.CanError as e:
            if running:  # Only print error if we *should* be running
                print(f"CAN Error: {e}")
            break  # Exit loop on any CAN error
        except Exception as e: #for catching Ctrl-C
            if running:
                print(f"CAN Error: {e}")
            break

def serial_logger(serial_port, log_file):
    """Logs serial port data to the specified file."""
    global running
    print("Serial logger started.")
    while running:
        try:
            if serial_port.in_waiting > 0:
                line = serial_port.readline().decode('utf-8', errors='replace').strip()
                log_file.write(f"SERIAL,{time.time():.6f},{line}\n")
        except serial.SerialException as e:
            if running: # Only print error if we *should* be running
                print(f"Serial Error: {e}")
            break #exit loop on Serial Exception.
        except Exception as e: #for catching Ctrl-C
            if running:
                print(f"Serial Error: {e}")
            break


def main():
    global can_bus, serial_port, running, log_file

    parser = argparse.ArgumentParser(description="Log CAN bus and serial data simultaneously.")
    parser.add_argument("--can_interface", default=CAN_INTERFACE, help=f"CAN interface name (default: {CAN_INTERFACE})")
    parser.add_argument("--can_type", default=CAN_INTERFACE_TYPE, help=f"CAN interface type (default: {CAN_INTERFACE_TYPE})")
    parser.add_argument("--can_bitrate", type=int, default=125000, help="CAN bus bitrate (default: 125000)")
    parser.add_argument("--serial_port", default=SERIAL_PORT, help=f"Serial port device (default: {SERIAL_PORT})")
    parser.add_argument("--baud_rate", type=int, default=BAUD_RATE, help=f"Serial port baud rate (default: {BAUD_RATE})")
    parser.add_argument("--log_dir", default=LOG_DIR, help=f"Directory to store log files (default: {LOG_DIR})")
    parser.add_argument("--log_prefix", default="can_serial_log", help="Prefix for log filenames (default: can_serial_log)")
    parser.add_argument("--no_can", action="store_true", help="Disable CAN logging")
    parser.add_argument("--no_serial", action="store_true", help="Disable serial logging")
    parser.add_argument("--canbox_debug_mode", action="store_true", help="Put the canbox into debug mode before logging. Requires --no_serial to be false.")

    args = parser.parse_args()


    if not os.path.exists(args.log_dir):
        os.makedirs(args.log_dir)

    timestamp = datetime.datetime.now().strftime("%Y%m%d_%H%M%S")
    log_filename = os.path.join(args.log_dir, f"{args.log_prefix}_{timestamp}.csv")
    log_file = open(log_filename, "w", buffering=1)  # Open with line buffering
    if not log_file:
        print(f"Error: Could not open log file: {log_filename}")
        return

    print(f"Logging to file: {log_filename}")
    log_file.write("Type,Timestamp,Data\n")  # Simple CSV header


    can_thread = None
    serial_thread = None

    if not args.no_can:
      can_bus = setup_can(args.can_interface, args.can_type, args.can_bitrate)
      if can_bus:
          can_thread = threading.Thread(target=can_logger, args=(can_bus, log_file))
          can_thread.start()

    if not args.no_serial:
      serial_port = setup_serial(args.serial_port, args.baud_rate)
      if serial_port:
        if args.canbox_debug_mode:
          print("Entering canbox debug mode...")
          try:
              # Send the debug mode command.  CRITICAL:  Ensure correct timing and character sequence!
              serial_port.write(b"OOOOOOOOOOOO")  # At least 10 'O' characters within 1 second
              time.sleep(0.5)  # Give the canbox time to respond
              print(serial_port.read_all().decode('utf-8', errors='ignore'))  # Read any immediate response
          except serial.SerialException as e:
                print(f"Error sending debug command: {e}")
                running = False # Prevent starting loggers if setup fails.
        serial_thread = threading.Thread(target=serial_logger, args=(serial_port, log_file))
        serial_thread.start()

    if not can_thread and not serial_thread:
        print("No logging threads started. Exiting.")
        log_file.close()
        return

    try:
        while running:
            time.sleep(0.1)  # Short sleep to prevent busy-waiting
    except KeyboardInterrupt:
        print("Stopping loggers...")
        running = False  # Signal threads to stop

    finally:
        if can_thread:
          can_thread.join()  # Wait for CAN thread to finish
        if serial_thread:
          serial_thread.join() # Wait for serial thread to finish

        if can_bus:
            can_bus.shutdown() #Added can bus shutdown
            os.system(f'sudo ip link set drv can0 down')  # Bring down the CAN interface
            os.system(f'sudo killall slcand  || true')  # Bring down the CAN interface

        if serial_port:
          serial_port.close()
        if log_file:
          log_file.close()
        print("Logging stopped.")

if __name__ == "__main__":
    main()
