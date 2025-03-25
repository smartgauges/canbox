import sys
import can
import serial
import time
import re
from threading import Thread

class CanboxKeywords:
    ROBOT_LIBRARY_SCOPE = 'SUITE'
    ROBOT_LISTENER_API_VERSION = 2

    def __init__(self, can_interface='vcan0', can_type='socketcan', serial_port='/tmp/uart', baudrate=38400):
        self.can_bus = None
        self.serial_port = None
        self.can_interface = can_interface
        self.can_type = can_type
        self.serial_port_path = serial_port
        self.baudrate = baudrate
        self._running = True #for correct handling of exit
        self._serial_data = ""


    def setup_can_bus(self, *args, **kwargs):
        """
        Initializes the CAN bus connection.
        """
        try:
            # Use the provided interface and type
            self.can_bus = can.interface.Bus(channel=self.can_interface, bustype=self.can_type, bitrate=125000)
            print(f"CAN bus on {self.can_interface} ({self.can_type}) initialized.")
        except can.CanError as e:
            raise Exception(f"Failed to initialize CAN bus: {e}")
        
    def close_can(self):
        """
        Closes can bus connection
        """
        if self.can_bus:
            self.can_bus.shutdown()

    def send_can_message_in_background(self, can_id, data, repeat=1, delay=0.1, is_extended=False):
        """
        Sends a CAN message in the background.

        Args:
            can_id  (int): The CAN ID (in hex or decimal).
            data (hex): The data payload (hex string, e.g., 'DEADBEEF').
            repeat (int): Number of times to repeat the message.
            delay (float): Delay in seconds between each message.
            is_extended: set to True if canId is extended
        """
        def send_message_loop(can_id, data, repeat, delay, is_extended):
            for _ in range(repeat):
                try:
                    self.send_can_message(can_id, data, is_extended)
                except ValueError as e:
                    print(f"Error sending CAN message: {e}")
                    break
                time.sleep(delay)

        self.thread = Thread(target=send_message_loop, args=(can_id, data, repeat, delay, is_extended))
        self.thread.daemon = True  # Daemonize thread
        self.thread.start()
        

    def send_can_message(self, can_id, data, is_extended=False):
        """
        Sends a CAN message.

        Args:
            can_id (int): The CAN ID (in hex or decimal).
            data (hex): The data payload (hex string, e.g., 'DEADBEEF').
            is_extended: set to True if canId is extended
        """
        try:
            can_id = int(can_id, 16)  # Ensure can_id is an integer
        except ValueError:
            raise ValueError(f"Invalid CAN ID: {can_id}. Must be a hexadecimal string.")

        try:
            data_bytes = int(data, 16).to_bytes(8, byteorder='big')
        except ValueError:
            raise ValueError(f"Invalid data: {data}. Must be a hexadecimal string.")

        msg = can.Message(arbitration_id=can_id, data=data_bytes, is_extended_id=is_extended)
        try:
            self.can_bus.send(msg)
            # print(f"Sent CAN message: ID={hex(can_id)}, Data={data}")
        except can.CanError as e:
            raise Exception(f"Failed to send CAN message: {e}")

    def wait_for_can_message(self, can_id, data=None, timeout=30, mask=None):
        """
        Waits for a specific CAN message with an optional data match and timeout.

        Args:
            can_id (str): The CAN ID to wait for (hex string, e.g., '0x123').
            data (str, optional): The expected data payload (hex string, e.g., 'DEADBEEF').
                If None, any data is accepted.
            timeout (float, optional): Timeout in seconds. Defaults to 5 seconds.
            mask (str, optional):  Hexadecimal mask to apply to the data before comparison.
                                   Must be the same length as `data` if provided.

        Returns:
            can.Message: The received CAN message if it matches, or None if no matching message is received
                         within the timeout.
        Raises:
             ValueError: on invalid input
        """
        try:
            can_id = int(can_id, 16)
        except ValueError:
            raise ValueError(f"Invalid CAN ID: {can_id}. Must be a hexadecimal string.")

        if data is not None:
            try:
                expected_data = bytes.fromhex(data)
            except ValueError:
                raise ValueError(f"Invalid data: {data}.  Must be a hexadecimal string.")
        else:
            expected_data = None

        if mask is not None:
            try:
                mask_bytes = bytes.fromhex(mask)
            except ValueError:
                 raise ValueError(f"Invalid mask: {mask}. Must be a hexadecimal string.")

            if expected_data is not None and len(mask_bytes) != len(expected_data):
                raise ValueError("Mask and data must have the same length.")
        else:
            mask_bytes = None


        start_time = time.time()
        while time.time() - start_time < timeout:
            msg = self.can_bus.recv(timeout=0.1) # Check frequently
            if msg is None: # important for not stucking in the loop
                continue

            if msg.arbitration_id == can_id:
                if expected_data is None:
                    return msg  # Return immediately if no data check is needed

                if len(msg.data) != len(expected_data):
                    continue # Skip if data lenghts are different

                if mask_bytes:
                    masked_data = bytes([msg.data[i] & mask_bytes[i] for i in range(len(msg.data))])
                    masked_expected = bytes([expected_data[i] & mask_bytes[i] for i in range(len(expected_data))])
                    if masked_data == masked_expected:
                        return msg  # Return message if data matches after applying mask
                elif msg.data == expected_data:
                    return msg  # Return the message if data matches
        return None  # Timeout

    def setup_serial(self):
        """
        Initializes the serial port connection.
        """
        try:
            self.serial_port = serial.Serial(self.serial_port_path, self.baudrate, timeout=30)
            print(f"Serial port {self.serial_port_path} opened at {self.baudrate} baud.")
        except serial.SerialException as e:
            raise Exception(f"Could not open serial port {self.serial_port_path}: {e}")

    def close_serial(self):
        """
        Closes the serial port.
        """
        if self.serial_port and self.serial_port.is_open:
            self.serial_port.close()
            self.serial_port = None
            print("Serial port closed.")

    def send_serial_command(self, command):
        """
        Sends a command over the serial port.

        Args:
            command (str): The command string to send.  Newline is added automatically.
        """

        if not self.serial_port or not self.serial_port.is_open:
            raise Exception("Serial port is not open.")
        try:
            command_bytes = (command + '\n').encode('utf-8')  # Encode to bytes, add newline
            self.serial_port.write(command_bytes)
            #print(f"Sent serial command: {command}")

        except serial.SerialException as e:
            raise Exception(f"Error sending serial command: {e}")

    def wait_for_serial_regex(self, pattern, timeout=5):
         """
         Waits for a line matching a regular expression on the serial port.
         Args:
             pattern (str): The regular expression pattern to match (as a string).
             timeout (float): Timeout in seconds.

         Returns:
             str: The matching line if found, None otherwise.
         """
         start_time = time.time()
         regex = re.compile(pattern)  # Compile the regex
         while time.time() - start_time < timeout:
             if self.serial_port.in_waiting > 0:
                 line = self.serial_port.readline().decode('utf-8', errors='replace').strip()
                 # print(f"Received serial data: {line}")
                 self._serial_data += line + "\n"
                 if regex.search(line):
                     print(f"Matched serial data: {line}")
                     return line
             time.sleep(0.01)  # Short delay to avoid busy-waiting, but check frequently
         
         raise AssertionError(f"❌ Failed to find serial data matching pattern: {pattern}")
         
    def get_serial_log(self):
        """
        Returns the accumulated serial data.
        """
        return self._serial_data

    def clear_serial_log(self):
        """
        Clears the accumulated serial data.
        """
        self._serial_data = ""

    def _close(self):
        """
        This method is automatically called when the library is closed.
        """
        if self.serial_port is not None:
            self.close_serial()
            self.serial_port = None  # Set to None after closing
        if self.can_bus is not None:
            self.close_can()
            self.can_bus = None   # Set to None after shutdown

    def stop_all_logging(self):
        """
        Stops the logging threads.
        """
        global running  # Access the global running flag
        running = False

    def __del__(self):
        """
        Ensure resources get cleaned up if the object is deleted.
        """
        self._close()