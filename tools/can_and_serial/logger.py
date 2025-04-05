#!/usr/bin/env python3

import can
import serial
import time
import threading
import datetime
import os
import argparse
import sys # Added for sys.stdout

# --- Configuration (Adjust as needed) ---
CAN_INTERFACE = 'can0'
SERIAL_PORT = '/dev/ttyACM0'
BAUD_RATE = 19200  #38400  # Default, overridden by args
LOG_DIR = "logs"
CAN_INTERFACE_TYPE = 'socketcan'

# --- RZC Specific ---
RZC_HEADER = 0xFD
RZC_MAX_FRAME_LEN = (1 + 1 + 1 + 30 + 1) # Based on your previous simulator script: HD+LEN+TYPE+DATA(max 30)+CS

# --- Global Variables ---
can_bus = None
serial_port = None
running = True
log_file = None

# --- Helper Functions ---

def setup_can(interface=CAN_INTERFACE, interface_type=CAN_INTERFACE_TYPE, bitrate=125000):
    """Sets up the CAN interface."""
    global can_bus
    try:
        print(f"Attempting to set up CAN interface {interface} ({interface_type}) at {bitrate}bps...")
        # This part is specific to linux using slcand for some adapters
        # You might need to adjust or remove these lines depending on your CAN hardware setup
        # If using a different setup (e.g., direct socketcan), configure it here.
        # os.system(f'sudo slcan_attach -f -s6 -o {interface}') # Example, might not be needed
        # os.system(f'sudo slcand -o -c -f -s{bitrate // 100000} {interface} {interface}') # Check slcand syntax
        # os.system(f'sudo ip link set {interface} down || true') # Ensure it's down first
        # os.system(f'sudo ip link set {interface} type can bitrate {bitrate}')
        # os.system(f'sudo ip link set {interface} up')
        time.sleep(0.1) # Give time for interface to come up

        can_bus = can.interface.Bus(channel=interface, bustype=interface_type, bitrate=bitrate)
        print(f"CAN interface {interface} set up successfully.")
        return can_bus
    except Exception as e:
        print(f"Error setting up CAN interface {interface}: {e}")
        print("CAN logging might be unavailable.")
        return None

def setup_serial(port=SERIAL_PORT, baudrate=BAUD_RATE):
    """Sets up the serial port."""
    global serial_port
    try:
        serial_port = serial.Serial(port, baudrate, timeout=0.05) # Use short timeout for non-blocking reads
        print(f"Serial port {port} set up at {baudrate} baud.")
        return serial_port
    except Exception as e:
        print(f"Error setting up serial port {port}: {e}")
        print("Serial logging might be unavailable.")
        return None


def can_logger(can_bus, log_file_handle):
    """Logs CAN bus messages to the specified file."""
    global running
    print("CAN logger started.")
    while running:
        try:
            message = can_bus.recv(0.1)  # Timeout of 0.1 seconds
            if message and running: # Check running again after potentially blocking recv
                timestamp = message.timestamp # Use the message's timestamp
                # Format for CSV
                can_id_hex = f"{message.arbitration_id:X}"
                dlc = message.dlc
                data_hex = ",".join(f"{byte:02X}" for byte in message.data)
                # Fill remaining data fields with empty strings if DLC < 8
                data_fields = data_hex.split(',')
                while len(data_fields) < 8:
                    data_fields.append("") # Add empty strings for missing bytes
                log_line = f"CAN,{timestamp:.6f},{can_id_hex},{dlc},{','.join(data_fields)}\n"
                log_file_handle.write(log_line)

        except can.CanError as e:
            if running:
                print(f"CAN Error: {e}")
            break # Exit loop on CAN error
        except Exception as e:
            if running:
                print(f"Unexpected CAN Error: {e}")
            break
    print("CAN logger stopped.")


# <<< MODIFIED serial_logger >>>
def serial_logger(serial_port, log_file_handle):
    """Logs serial port data, printing each frame starting with FD in hex format."""
    global running
    print("Serial logger started (Hex Mode).")
    read_buffer = bytearray()

    while running:
        new_data = None
        try:
            # Read available bytes non-blockingly
            if serial_port.in_waiting > 0:
                bytes_to_read = serial_port.in_waiting
                new_data = serial_port.read(bytes_to_read)
                if new_data:
                    read_buffer.extend(new_data)

            # Process buffer for complete RZC frames (FD LEN DTYPE ... CS)
            while True:
                start_index = read_buffer.find(RZC_HEADER)
                if start_index == -1:
                    # No header found, keep the last few bytes in case header is split
                    if len(read_buffer) > 2: # Keep potential LEN byte if header was split
                         read_buffer = read_buffer[-2:]
                    break # Need more data

                # Discard bytes before the header
                if start_index > 0:
                    #print(f"SERIAL DEBUG: Discarding {start_index} bytes before FD: {read_buffer[:start_index].hex(' ').upper()}")
                    read_buffer = read_buffer[start_index:]

                # Check if we have enough bytes for header and length
                if len(read_buffer) < 2:
                    break # Need more data for length byte

                # Get the length byte (RZC spec: LEN = 1 (DTYPE) + n (DATA))
                length_byte = read_buffer[1]
                data_payload_size = length_byte - 1
                # Expected full frame size = 1(FD) + 1(LEN) + length_byte(DTYPE+DATA) + 1(CS)
                expected_frame_size = 3 + length_byte

                # Basic validation of length byte
                if length_byte == 0 or data_payload_size < 0 or expected_frame_size > RZC_MAX_FRAME_LEN + 3:
                     # print(f"SERIAL DEBUG: Invalid LEN byte {length_byte:02X}. Discarding FD header.")
                     read_buffer.pop(0) # Discard the FD and try again
                     continue # Restart search in the loop

                # Check if we have enough bytes for the complete frame
                if len(read_buffer) < expected_frame_size:
                    break # Need more data for the full frame

                # Extract the complete frame
                frame_bytes = read_buffer[:expected_frame_size]
                timestamp = time.time() # Timestamp when frame is fully received

                # --- Optional: Checksum Validation ---
                # datatype_byte = frame_bytes[2]
                # data_bytes_payload = frame_bytes[3 : 3 + data_payload_size]
                # received_cs = frame_bytes[expected_frame_size - 1]
                # calculated_cs = calculate_rzc_checksum(length_byte, datatype_byte, data_bytes_payload)
                # if calculated_cs != received_cs:
                #     print(f"SERIAL WARNING: Checksum mismatch! Frame: {frame_bytes.hex(' ').upper()} CalcCS: {calculated_cs:02X} RcvdCS: {received_cs:02X}")
                # --- End Optional Checksum ---

                # Convert the full frame to hex string with spaces
                hex_frame = frame_bytes.hex(' ').upper()

                # Write to log
                log_line = f"SERIAL,{timestamp:.6f},{hex_frame}\n"
                log_file_handle.write(log_line)

                # Remove the processed frame from the buffer
                read_buffer = read_buffer[expected_frame_size:]
                # Continue loop to check for more frames in the remaining buffer


        except serial.SerialException as e:
            if running:
                print(f"Serial Error: {e}. Attempting reconnect...")
                serial_port.close()
                time.sleep(2)
                try:
                    serial_port.open()
                    print("Reconnected.")
                    read_buffer = bytearray() # Clear buffer
                except serial.SerialException:
                    print("Reconnect failed. Stopping serial logger.")
                    running = False # Stop if reconnect fails persistently
            break
        except Exception as e:
            if running:
                print(f"Unexpected Serial Error: {e}")
            break

    print("Serial logger stopped.")
# <<< END MODIFIED serial_logger >>>


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

    # Validate canbox_debug_mode usage
    if args.canbox_debug_mode and args.no_serial:
        print("Error: --canbox_debug_mode requires serial logging to be enabled (cannot use --no_serial).")
        sys.exit(1)
    if args.canbox_debug_mode and args.baud_rate != 38400:
        print("Warning: --canbox_debug_mode typically requires 38400 baud. Your specified baud rate might not work for entering debug mode.")
        # Proceed anyway, maybe user knows what they're doing


    if not os.path.exists(args.log_dir):
        try:
            os.makedirs(args.log_dir)
        except OSError as e:
            print(f"Error creating log directory '{args.log_dir}': {e}")
            sys.exit(1)

    timestamp = datetime.datetime.now().strftime("%Y%m%d_%H%M%S")
    log_filename = os.path.join(args.log_dir, f"{args.log_prefix}_{timestamp}.csv")

    try:
        log_file = open(log_filename, "w", buffering=1, encoding='utf-8', newline='')  # Open with line buffering
        print(f"Logging to file: {log_filename}")
        log_file.write("Type,Timestamp,Data\n")  # Simple CSV header
    except IOError as e:
        print(f"Error: Could not open log file '{log_filename}': {e}")
        sys.exit(1)


    can_thread = None
    serial_thread = None

    if not args.no_can:
      can_bus = setup_can(args.can_interface, args.can_type, args.can_bitrate)
      if can_bus:
          can_thread = threading.Thread(target=can_logger, args=(can_bus, log_file), daemon=True)
          can_thread.start()

    if not args.no_serial:
      serial_port = setup_serial(args.serial_port, args.baud_rate)
      if serial_port:
        if args.canbox_debug_mode:
          # Store original baud rate, switch to 38400 for debug command
          original_baud = args.baud_rate
          if serial_port.baudrate != 38400:
              print("Temporarily switching to 38400 baud for debug command...")
              try:
                  serial_port.baudrate = 38400
              except Exception as e:
                  print(f"Failed to set baud rate to 38400: {e}")
                  # Decide how to handle: exit or continue? Let's try to continue with original baud.
                  serial_port.baudrate = original_baud

          if serial_port.baudrate == 38400:
              print("Entering canbox debug mode...")
              try:
                  serial_port.write(b"OOOOOOOOOOOO")  # Ensure bytes
                  time.sleep(0.5)
                  response = serial_port.read_all()
                  if response:
                      print(f"Debug mode response (raw): {response!r}")
                      print(f"Debug mode response (decoded): {response.decode('utf-8', errors='replace')}")
                  else:
                      print("No immediate response to debug command.")
              except serial.SerialException as e:
                    print(f"Error sending debug command: {e}")
                    running = False
              # Switch back to the requested baud rate for logging
              if original_baud != 38400:
                  print(f"Switching back to {original_baud} baud for logging...")
                  try:
                      serial_port.baudrate = original_baud
                  except Exception as e:
                      print(f"Failed to set baud rate back to {original_baud}: {e}. Stopping serial logger.")
                      running = False # Stop if we can't set the correct logging baudrate
          else:
              print("Could not set 38400 baud, skipping debug command.")


        if running: # Only start logger thread if no critical errors occurred
            serial_thread = threading.Thread(target=serial_logger, args=(serial_port, log_file), daemon=True)
            serial_thread.start()

    if not can_thread and not serial_thread:
        print("Error: Neither CAN nor Serial logging could be started. Exiting.")
        log_file.close()
        if can_bus: can_bus.shutdown()
        if serial_port: serial_port.close()
        # Attempt to bring down CAN interface if setup was attempted
        # if not args.no_can:
            # os.system(f'sudo ip link set {args.can_interface} down || true')
        sys.exit(1)

    try:
        # Keep main thread alive while logging threads run
        while running:
            # Check if threads are alive (optional)
            if can_thread and not can_thread.is_alive():
                print("CAN logger thread terminated unexpectedly.")
                running = False
            if serial_thread and not serial_thread.is_alive():
                print("Serial logger thread terminated unexpectedly.")
                running = False
            time.sleep(0.5)
    except KeyboardInterrupt:
        print("\nCtrl+C detected. Stopping loggers...")
        running = False

    finally:
        print("Waiting for logging threads to finish...")
        if can_thread:
          can_thread.join(timeout=1.0)
        if serial_thread:
          serial_thread.join(timeout=1.0)

        if can_bus:
            try:
                can_bus.shutdown()
                print("CAN bus shutdown.")
            except Exception as e:
                print(f"Error during CAN bus shutdown: {e}")
            # Attempt to bring down the interface
            # os.system(f'sudo ip link set {args.can_interface} down || true')

        if serial_port:
            try:
                serial_port.close()
                print("Serial port closed.")
            except Exception as e:
                print(f"Error closing serial port: {e}")
        if log_file:
            try:
                log_file.close()
                print("Log file closed.")
            except Exception as e:
                print(f"Error closing log file: {e}")

        print("Logging stopped.")

if __name__ == "__main__":
    main()