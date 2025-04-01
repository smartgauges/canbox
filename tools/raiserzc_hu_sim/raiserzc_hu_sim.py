import serial
import time
import argparse
import signal
import os
import sys
from collections import defaultdict

# --- RZC Protocol Constants ---
RZC_HEADER = 0xFD
BAUD_RATE = 19200 # RZC Protocol uses 19200

# DataType Definitions (Slave -> Host) - Add more as needed
RZC_DTYPE_BUTTON_CMD = 0x02
RZC_DTYPE_AC_INFO = 0x21
RZC_DTYPE_WHEEL_ANGLE = 0x29
RZC_DTYPE_RADAR_ALL = 0x30 # Less likely used based on spec focus
RZC_DTYPE_RADAR_REVERSE = 0x32
RZC_DTYPE_TRIP_PAGE0 = 0x33
RZC_DTYPE_TRIP_PAGE1 = 0x34
RZC_DTYPE_TRIP_PAGE2 = 0x35
RZC_DTYPE_OUTSIDE_TEMP = 0x36
RZC_DTYPE_VEHICLE_STATUS = 0x38
RZC_DTYPE_VERSION_INFO = 0x7F
# Add other DataTypes here if you implement their decoding

# --- Global State ---
ser = None # Serial port object
running = True # Flag to control main loop
vehicle_state = defaultdict(lambda: "N/A") # Dictionary to store last known values
last_update_time = 0
display_refresh_interval = 0.2 # Refresh display every 0.2 seconds

# --- Graceful Exit ---
def signal_handler(sig, frame):
    global running
    print("\nStopping head unit simulator...")
    running = False
signal.signal(signal.SIGINT, signal_handler) # Handle Ctrl+C

# --- Checksum Calculation ---
def calculate_rzc_checksum(length_byte, datatype_byte, data_bytes):
    """Calculates the RZC protocol checksum."""
    chksum = 0
    chksum += length_byte
    chksum += datatype_byte
    for byte in data_bytes:
        chksum += byte
    return chksum & 0xFF # Return lower 8 bits

# --- Decoding Functions ---
def decode_button(data):
    if len(data) < 2: return "Invalid Button Data"
    key_code = data[0]
    status = data[1]
    # Map key code to name (Add more from spec)
    key_map = {
        0x02: "Menu", 0x03: "Up", 0x04: "Down", 0x07: "OK", 0x08: "Esc",
        0x10: "Mode", 0x11: "Source/Phone", 0x12: "Seek+", 0x13: "Seek-",
        0x14: "Vol+", 0x15: "Vol-", 0x16: "Mute", 0x17: "Memo Up", 0x18: "Memo Down",
        0x20: "Page SW", 0x23: "Bluetooth", 0x29: "Voice Cmd", 0x30: "Tel On",
        0x31: "Tel Off", 0x54: "DARK", 0x60: "Check", 0x80: "Power",
        # Add codes mapped in canbox for panel buttons like CLIM if needed
        0x0E: "DARK (Mapped)", 0x20: "CLIM (Mapped)"
    }
    key_name = key_map.get(key_code, f"Key {key_code:02X}")
    status_str = "Pressed" if status == 0x01 else "Released" if status == 0x00 else f"Status {status:02X}"
    return f"{key_name} {status_str}"

def decode_temperature(data):
    if not data: return "N/A"
    temp_raw = data[0]
    if temp_raw == 0xFF: return "Invalid"
    sign = -1 if (temp_raw & 0x80) else 1
    value = temp_raw & 0x7F
    return f"{sign * value} C"

def decode_radar(data):
    if len(data) < 7: return "Invalid Radar Data" # For 0x32
    status_map = {0x00: "Active", 0x01: "Active(NoDisp)", 0x02: "Active+Disp", 0x03: "Disabled"}
    status = status_map.get(data[0], f"Unknown({data[0]:02X})")
    # Map 0-5 level to string/bars (0=closest 5 bars, 5=inactive no bars)
    dist_map = {0: "|||||", 1: "||||-", 2: "|||--", 3: "||---", 4: "|----", 5: "-----"}
    rl = dist_map.get(data[1], "?")
    rm = dist_map.get(data[2], "?")
    rr = dist_map.get(data[3], "?")
    fl = dist_map.get(data[4], "?")
    fm = dist_map.get(data[5], "?")
    fr = dist_map.get(data[6], "?")
    return f"Status: {status} | R:[{rl} {rm} {rr}] F:[{fl} {fm} {fr}]"

def decode_status(data):
    if len(data) < 6: return "Invalid Status Data"
    # Data0 (Doors)
    doors = []
    if data[0] & 0x80: doors.append("FL")
    if data[0] & 0x40: doors.append("FR")
    if data[0] & 0x20: doors.append("RL")
    if data[0] & 0x10: doors.append("RR")
    if data[0] & 0x08: doors.append("Trunk")
    # Bonnet missing in this spec for 0x38?
    door_str = " ".join(doors) if doors else "All Closed"

    # Data1 (Settings/Status 1)
    park_assist = "On" if data[1] & 0x08 else "Off"

    # Data3 (Settings/Status 3)
    reverse = "R" if data[3] & 0x04 else "-"
    park_brake = "PB" if data[3] & 0x02 else "--"
    park_lights = "P" if data[3] & 0x01 else "-"

    # Combine into a readable string
    return f"Doors: {door_str} | ParkAs: {park_assist} | Gear: {reverse} | {park_brake} | ParkLt: {park_lights}"

def decode_trip(data, page):
     if len(data) < (9 if page == 0 else 6): return f"Invalid Trip{page} Data"
     prefix = f"Trip{page}: "
     result = []

     if page == 0: # Instantaneous + Range
         cons_raw = (data[0] << 8) | data[1]
         cons_str = "--.-" if cons_raw == 0xFFFF else f"{cons_raw / 10.0:.1f}L/100" # VERIFY SCALING!
         range_raw = (data[2] << 8) | data[3]
         range_str = "----" if range_raw == 0xFFFF else f"{range_raw}km"
         result.append(f"Inst:{cons_str}")
         result.append(f"Range:{range_str}")
         # Ignore Dest Range (data 4-5), Ignore Start/Stop time (data 6-8)
     else: # Trip 1 or 2
         cons_raw = (data[0] << 8) | data[1]
         cons_str = "--.-" if cons_raw == 0xFFFF else f"{cons_raw / 10.0:.1f}L/100" # VERIFY SCALING!
         speed_raw = (data[2] << 8) | data[3]
         speed_str = "---" if speed_raw == 0xFFFF else f"{speed_raw}km/h"
         dist_raw = (data[4] << 8) | data[5]
         dist_str = "----" if dist_raw == 0xFFFF else f"{dist_raw}km"
         result.append(f"AvgC:{cons_str}")
         result.append(f"AvgS:{speed_str}")
         result.append(f"Dist:{dist_str}")

     return prefix + " ".join(result)

def decode_wheel_angle(data):
    if len(data) < 2: return "N/A"
    raw_angle = int.from_bytes(bytes([data[1], data[0]]), byteorder='big', signed=True)
    # Spec says -5450 to +5450. Maybe degrees*10?
    # Display raw value for now, scaling needs verification
    return f"{raw_angle}" # Display raw value for now

# --- Main Display Function ---
def display_state(state):
    """Clears console and prints the current vehicle state."""
    os.system('cls' if os.name == 'nt' else 'clear') # Clear console
    print("--- RZC Head Unit Simulator ---")
    print(f"Timestamp: {time.strftime('%Y-%m-%d %H:%M:%S')}")
    print("-" * 30)
    # Print items in a somewhat logical order
    print(f"  Ignition : {state.get('IGN', 'N/A')} | ACC: {state.get('ACC', 'N/A')}")
    print(f"  Lights   : Park({state.get('Park Lights', 'N/A')}) Near({state.get('Near Lights', 'N/A')}) | Illum: {state.get('Illum Val', 'N/A')} ({state.get('Illum St', 'N/A')})")
    print(f"  Doors    : {state.get('Door Status', 'N/A')}")
    print(f"  Status   : Gear({state.get('Gear', 'N/A')}) Brake({state.get('Park Brake', 'N/A')}) Belt({state.get('Seat Belt', 'N/A')})")
    print(f"  Steering : {state.get('Wheel Angle', 'N/A')}")
    print(f"  Speed    : {state.get('Speed', 'N/A')} km/h | RPM: {state.get('RPM', 'N/A')}")
    print(f"  Odometer : {state.get('Odometer', 'N/A')} km")
    print(f"  Temps    : Outside({state.get(RZC_DTYPE_OUTSIDE_TEMP, 'N/A')}) Coolant({state.get('Coolant', 'N/A')}) Oil({state.get('Oil', 'N/A')})")
    print(f"  Fuel     : {state.get('Fuel Level', 'N/A')}% | Low: {state.get('Low Fuel', 'N/A')}")
    print(f"  Voltage  : {state.get('Voltage', 'N/A')} V | Low: {state.get('Low Voltage', 'N/A')}")
    print(f"  Trip 0   : {state.get(RZC_DTYPE_TRIP_PAGE0, 'N/A')}")
    print(f"  Trip 1   : {state.get(RZC_DTYPE_TRIP_PAGE1, 'N/A')}")
    print(f"  Trip 2   : {state.get(RZC_DTYPE_TRIP_PAGE2, 'N/A')}")
    print(f"  Radar    : {state.get(RZC_DTYPE_RADAR_REVERSE, 'N/A')}") # Assuming 0x32 is primary
    print(f"  AC Info  : {state.get(RZC_DTYPE_AC_INFO, 'N/A')}")
    print(f"  Last Btn : {state.get(RZC_DTYPE_BUTTON_CMD, 'N/A')}")
    print("-" * 30)
    print("Press Ctrl+C to exit.")

# --- Main Loop ---
def main(port):
    global ser, running, vehicle_state, last_update_time
    try:
        ser = serial.Serial(port, BAUD_RATE, timeout=0.1) # Short timeout for non-blocking read attempt
        print(f"Connected to {port} at {BAUD_RATE} baud.")
    except serial.SerialException as e:
        print(f"Error opening serial port {port}: {e}")
        sys.exit(1)

    read_buffer = bytearray()
    needs_update = False

    while running:
        # Read available data
        try:
            if ser.in_waiting > 0:
                data = ser.read(ser.in_waiting)
                read_buffer.extend(data)
        except serial.SerialException as e:
            print(f"Serial error: {e}. Attempting to reconnect...")
            ser.close()
            time.sleep(2)
            try:
                ser = serial.Serial(port, BAUD_RATE, timeout=0.1)
                print(f"Reconnected to {port}.")
                read_buffer = bytearray() # Clear buffer on reconnect
            except serial.SerialException:
                print(f"Reconnect failed. Exiting.")
                running = False
                break # Exit loop if reconnect fails
            continue

        # Process buffer for complete frames
        while len(read_buffer) >= 4: # Minimum frame size (FD LEN DTYPE CS) -> LEN=1
            if read_buffer[0] != RZC_HEADER:
                read_buffer.pop(0) # Discard invalid header byte
                continue

            msg_len_byte = read_buffer[1] # Length byte (DataType + Data)
            data_payload_size = msg_len_byte - 1
            expected_frame_size = 1 + 1 + msg_len_byte + 1 # HD+LEN+(DTYPE+DATA)+CS

            if data_payload_size < 0 or msg_len_byte < 1: # Invalid length byte
                 print(f"Invalid frame length: {msg_len_byte}. Discarding header.")
                 read_buffer.pop(0)
                 continue

            if len(read_buffer) < expected_frame_size:
                break # Need more data for this potential frame

            # We have enough bytes for a potential full frame
            frame_bytes = read_buffer[:expected_frame_size]
            datatype_byte = frame_bytes[2]
            data_bytes = frame_bytes[3 : 3 + data_payload_size]
            received_cs = frame_bytes[expected_frame_size - 1]

            # Verify checksum
            calculated_cs = calculate_rzc_checksum(msg_len_byte, datatype_byte, data_bytes)

            if calculated_cs == received_cs:
                # Frame is valid, decode based on DataType
                decoded_info = f"Raw: {' '.join(f'{b:02X}' for b in data_bytes)}" # Default
                try:
                    if datatype_byte == RZC_DTYPE_BUTTON_CMD:
                        decoded_info = decode_button(data_bytes)
                    elif datatype_byte == RZC_DTYPE_OUTSIDE_TEMP:
                        decoded_info = decode_temperature(data_bytes)
                    elif datatype_byte == RZC_DTYPE_RADAR_REVERSE:
                        decoded_info = decode_radar(data_bytes)
                    elif datatype_byte == RZC_DTYPE_VEHICLE_STATUS:
                        decoded_info = decode_status(data_bytes)
                        # Also extract individual items for better display
                        if len(data_bytes) >= 4:
                             vehicle_state['Door Status'] = f"FL:{'O' if data_bytes[0]&0x80 else 'C'} FR:{'O' if data_bytes[0]&0x40 else 'C'} RL:{'O' if data_bytes[0]&0x20 else 'C'} RR:{'O' if data_bytes[0]&0x10 else 'C'} T:{'O' if data_bytes[0]&0x08 else 'C'}"
                             vehicle_state['Park Lights'] = "ON" if data_bytes[3] & 0x01 else "OFF"
                             vehicle_state['Park Brake'] = "ON" if data_bytes[3] & 0x02 else "OFF"
                             vehicle_state['Gear'] = "R" if data_bytes[3] & 0x04 else "-" # Very basic gear info from status
                    elif datatype_byte == RZC_DTYPE_TRIP_PAGE0:
                        decoded_info = decode_trip(data_bytes, 0)
                    elif datatype_byte == RZC_DTYPE_TRIP_PAGE1:
                        decoded_info = decode_trip(data_bytes, 1)
                    elif datatype_byte == RZC_DTYPE_TRIP_PAGE2:
                        decoded_info = decode_trip(data_bytes, 2)
                    elif datatype_byte == RZC_DTYPE_WHEEL_ANGLE:
                        decoded_info = decode_wheel_angle(data_bytes)
                    # Add elif for other DataTypes (0x21, etc.)
                    elif datatype_byte == RZC_DTYPE_VERSION_INFO:
                         try:
                             decoded_info = f"Version: {data_bytes.decode('ascii', errors='ignore')}"
                         except:
                             decoded_info = "Version: Decode Error"

                    # Store the decoded string representation
                    vehicle_state[datatype_byte] = decoded_info
                    needs_update = True
                except Exception as e:
                     print(f"Error decoding DType {datatype_byte:02X}: {e}")

                # Consume the processed frame from the buffer
                read_buffer = read_buffer[expected_frame_size:]

            else:
                print(f"Checksum mismatch! Frame: {' '.join(f'{b:02X}' for b in frame_bytes)} CalcCS: {calculated_cs:02X} RcvdCS: {received_cs:02X}. Discarding header.")
                read_buffer.pop(0) # Discard the invalid header (0xFD)

        # Refresh display periodically or immediately after update
        current_time = time.time()
        if needs_update or (current_time - last_update_time > display_refresh_interval):
             display_state(vehicle_state)
             last_update_time = current_time
             needs_update = False

        # Small sleep to prevent busy-looping if no data
        if not ser.in_waiting:
             time.sleep(0.01)


    if ser and ser.is_open:
        ser.close()
        print(f"Serial port {port} closed.")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="RZC PSA CANbox Head Unit Simulator")
    parser.add_argument("port", help="Serial port connected to the CANbox (e.g., /dev/ttyUSB0, COM3)")
    parser.add_argument("-b", "--baud", type=int, default=BAUD_RATE, help=f"Baud rate (default: {BAUD_RATE})")
    args = parser.parse_args()

    # Update baud rate if specified
    BAUD_RATE = args.baud

    main(args.port)