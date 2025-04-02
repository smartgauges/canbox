# tests/robot/resources/CanboxKeywords.py

import sys
import can
import serial
import time
import re
from threading import Condition, Thread
import telnetlib


c = Condition()
kill_can_threads = 0      #shared between Thread_A and Thread_B

class CanboxKeywords:
    ROBOT_LIBRARY_SCOPE = 'SUITE'
    ROBOT_LISTENER_API_VERSION = 2

    # --- NEW: Monitor Constants ---
    DEFAULT_MONITOR_HOST = "localhost"
    DEFAULT_MONITOR_PORT = 3456 # Match the port in .resc
    MONITOR_TIMEOUT = 5 # Timeout for telnet commands

    def __init__(self, can_interface='vcan0', can_type='socketcan',
                 serial_port='/tmp/uart', baudrate=38400,
                 monitor_port=DEFAULT_MONITOR_PORT): # Added monitor_port
        self.can_bus = None
        # Main debug/command serial port
        self.serial_port_cmd = None
        self.serial_port_cmd_path = serial_port
        self.baudrate = baudrate
        # Renode Monitor Telnet Connection (NEW)
        self.monitor_telnet = None
        self.monitor_host = self.DEFAULT_MONITOR_HOST
        self.monitor_port = monitor_port
        # CAN settings
        self.can_interface = can_interface
        self.can_type = can_type
        # State variables
        self._running = True
        self._serial_cmd_data = ""

    # --- CAN Methods (Keep as they are) ---
    # ... setup_can_bus, close_can, send_can_message_in_background, send_can_message, wait_for_can_message ...
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
    
    # Add this method inside the CanboxKeywords class in CanboxKeywords.py
    def calculate_rzc_checksum(self, length_hex, datatype_hex, data_hex=""):
        """Calculates the RZC PSA checksum (Sum(LEN, DTYPE, DATA) & 0xFF).

        Args:
            length_hex (str): The LEN byte as a hex string (e.g., '04').
            datatype_hex (str): The DataType byte as a hex string (e.g., '02').
            data_hex (str): The DATA bytes as a hex string (e.g., '140100'). Defaults to "".

        Returns:
            str: The calculated checksum as a two-digit uppercase hex string (e.g., '1B').
        """
        try:
            length = int(length_hex, 16)
            datatype = int(datatype_hex, 16)
            data_bytes = bytes.fromhex(data_hex) if data_hex else b''

            checksum = 0
            checksum = (checksum + length) & 0xFF
            checksum = (checksum + datatype) & 0xFF
            for byte in data_bytes:
                checksum = (checksum + byte) & 0xFF

            return f"{checksum:02X}"
        except ValueError as e:
            raise ValueError(f"Error calculating RZC checksum: Invalid hex input - {e}")


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
                c.acquire()
                if kill_can_threads:
                    c.release()
                    break
                c.release()
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

    # --- Serial Port Methods ---
    def setup_serial(self):
        """Initializes the main command/debug serial port connection."""
        try:
            self.serial_port_cmd = serial.Serial(self.serial_port_cmd_path, self.baudrate, timeout=1) # Use specific name
            print(f"Command serial port {self.serial_port_cmd_path} opened at {self.baudrate} baud.")
        except serial.SerialException as e:
            raise Exception(f"Could not open command serial port {self.serial_port_cmd_path}: {e}")

    def close_serial(self):
        """Closes the main command/debug serial port."""
        if self.serial_port_cmd and self.serial_port_cmd.is_open:
            self.serial_port_cmd.close()
            self.serial_port_cmd = None
            print("Command serial port closed.")

    def send_serial_command(self, command):
        """Sends a command over the main command serial port."""
        if not self.serial_port_cmd or not self.serial_port_cmd.is_open:
            raise Exception("Command serial port is not open.")
        try:
            time.sleep(0.1) # Ensure previous output is processed
            command_bytes = (command + '\n').encode('utf-8')
            self.serial_port_cmd.write(command_bytes)
            time.sleep(0.1) # Allow command to be processed
        except serial.SerialException as e:
            raise Exception(f"Error sending serial command: {e}")

    def wait_for_serial_regex(self, pattern, timeout=5):
        """Waits for a line matching a regex on the COMMAND serial port."""
        # ... (Keep the improved version from the previous response) ...
        start_time = time.time()
        regex = re.compile(pattern)
        buffered_data = ""
        try:
            if self.serial_port_cmd and self.serial_port_cmd.in_waiting > 0:
                 buffered_data = self.serial_port_cmd.read(self.serial_port_cmd.in_waiting).decode('utf-8', errors='replace')
                 self._serial_cmd_data += buffered_data # Add to log
        except (serial.SerialException, OSError):
            pass

        for line in buffered_data.splitlines():
             if regex.search(line.strip()):
                 print(f"Matched buffered serial data: {line.strip()}")
                 return line.strip()

        while time.time() - start_time < timeout:
            try:
                if self.serial_port_cmd and self.serial_port_cmd.in_waiting > 0:
                    line = self.serial_port_cmd.readline().decode('utf-8', errors='replace').strip()
                    self._serial_cmd_data += line + "\n"
                    if regex.search(line):
                        print(f"Matched serial data: {line}")
                        self.kill_can_threads = 1
                        return line
            except (serial.SerialException, OSError) as e:
                print(f"Warning: Serial exception during read: {e}")
                if not self.serial_port_cmd or not self.serial_port_cmd.is_open:
                     print("Command serial port closed unexpectedly, attempting to reopen...")
                     try: # Add try block for reopening
                       self.setup_serial() # Try to reopen
                       if not self.serial_port_cmd:
                            raise AssertionError(f"❌ Command Serial port disconnected and could not be reopened.")
                     except Exception as reopen_e:
                          raise AssertionError(f"❌ Failed to reopen command serial port: {reopen_e}")

            time.sleep(0.01)

        print(f"Timeout reached. Checking accumulated log for pattern: {pattern}")
        for line in self._serial_cmd_data.splitlines():
             if regex.search(line.strip()):
                 print(f"Matched accumulated serial data: {line.strip()}")
                 self.kill_can_threads = 1
                 return line.strip()

        raise AssertionError(f"❌ Failed to find command serial data matching pattern: {pattern} within {timeout}s. Full log:\n{self._serial_cmd_data}")


    def get_serial_log(self):
        """Returns the accumulated command serial data."""
        return self._serial_cmd_data

    def clear_serial_log(self):
        """Clears the accumulated command serial data."""
        self._serial_cmd_data = ""

    # --- NEW: Renode Monitor Interaction Keywords ---
    def connect_to_renode_monitor(self):
        """Connects to the Renode monitor via Telnet."""
        if self.monitor_telnet is None:
            try:
                print(f"Connecting to Renode monitor at {self.monitor_host}:{self.monitor_port}...")
                self.monitor_telnet = telnetlib.Telnet(self.monitor_host, self.monitor_port, timeout=self.MONITOR_TIMEOUT)
                # Read initial welcome message or prompt
                output = self.monitor_telnet.read_until(b"(monitor)", timeout=self.MONITOR_TIMEOUT).decode('utf-8', errors='replace')
                print(f"Connected to Renode monitor. Initial output:\n{output}")
            except Exception as e:
                raise ConnectionError(f"Failed to connect to Renode monitor at {self.monitor_host}:{self.monitor_port}: {e}")

    def disconnect_from_renode_monitor(self):
        """Disconnects from the Renode monitor."""
        if self.monitor_telnet:
            try:
                self.monitor_telnet.close()
                print("Disconnected from Renode monitor.")
            except Exception as e:
                print(f"Warning: Error disconnecting from Renode monitor: {e}")
            finally:
                self.monitor_telnet = None

    def execute_monitor_command(self, command, timeout=MONITOR_TIMEOUT):
        """Executes a command on the Renode monitor and returns the output."""
        if not self.monitor_telnet:
            self.connect_to_renode_monitor() # Auto-connect if not already connected

        try:
            # print(f"Executing monitor command: {command}") # Debug
            self.monitor_telnet.write(command.encode('utf-8') + b"\n")
            # Read until the prompt returns, capturing the output
            output = self.monitor_telnet.read_until(b"(monitor)", timeout=timeout).decode('utf-8', errors='replace')
            # Clean up the output: remove the command echo and the final prompt
            lines = output.splitlines()
            if lines and lines[0].strip() == command: # Remove command echo if present
                lines = lines[1:]
            if lines and lines[-1].strip() == "(monitor)": # Remove trailing prompt
                lines = lines[:-1]
            result = "\n".join(lines).strip()
            # print(f"Monitor command result: {result}") # Debug
            return result
        except EOFError:
             raise ConnectionAbortedError("Renode monitor connection closed unexpectedly.")
        except Exception as e:
            raise RuntimeError(f"Error executing Renode monitor command '{command}': {e}")

    def read_gpio_register(self, gpio_peripheral, register_offset=0x0C):
        """
        Reads a specific GPIO register (default ODR) via the Renode monitor.

        Args:
            gpio_peripheral (str): The name of the GPIO peripheral in Renode (e.g., "sysbus.gpioB").
            register_offset (int): The byte offset of the register to read (default 0x0C for ODR).

        Returns:
            int: The value of the register.
        Raises:
            ValueError: If the output cannot be parsed as a number.
            RuntimeError: If the monitor command fails.
        """
        command = f" {gpio_peripheral} ReadDoubleWord {register_offset}"
        output = self.execute_monitor_command(command)
        # Output is usually just the hex number, e.g., "0x200"
        try:
            # Find the hexadecimal value in the output (robust parsing)
            match = re.search(r'0x[0-9a-fA-F]+', output)
            if match:
                return int(match.group(0), 16)
            else:
                 raise ValueError(f"Could not parse register value from monitor output: '{output}'")
        except ValueError as e:
             raise ValueError(f"Error parsing register value from monitor output '{output}': {e}")


    def get_gpio_pin_state_directly(self, gpio_peripheral, pin_number):
        """
        Reads the ODR for the peripheral and returns the state of a specific pin.

        Args:
            gpio_peripheral (str): Renode name (e.g., "sysbus.gpioB").
            pin_number (int): The pin number (0-15).

        Returns:
            int: 0 if the pin is low, 1 if the pin is high.
        """
        if not (0 <= pin_number <= 15):
             raise ValueError("Pin number must be between 0 and 15.")

        register_value = self.read_gpio_register(gpio_peripheral, 0x0C) # Read ODR
        pin_mask = 1 << pin_number
        return 1 if (register_value & pin_mask) else 0

    def gpio_pin_should_be_directly(self, gpio_peripheral, pin_number, expected_state, timeout=2.0, polling_interval=0.1):
        """
        Waits until a GPIO pin reaches the expected state by directly reading registers via monitor.

        Args:
            gpio_peripheral (str): Renode name (e.g., "sysbus.gpioB").
            pin_number (int): The pin number (0-15).
            expected_state (int): 0 for low, 1 for high.
            timeout (float): Max time to wait in seconds.
            polling_interval(float): How often to check the register.
        """
        pin_number = int(pin_number)
        expected_state = int(expected_state)

        start_time = time.time()
        last_state = -1 # Initialize to an invalid state
        while time.time() - start_time < timeout:
            current_state = self.get_gpio_pin_state_directly(gpio_peripheral, pin_number)
            if current_state != last_state:
                 print(f"Direct GPIO Check - {gpio_peripheral} Pin {pin_number}: {current_state}")
                 last_state = current_state

            if current_state == expected_state:
                print(f"✅ Direct GPIO - {gpio_peripheral} Pin {pin_number} reached state {expected_state}")
                return
            time.sleep(polling_interval)

        # Timeout failure
        final_state = self.get_gpio_pin_state_directly(gpio_peripheral, pin_number) # Get final state
        raise AssertionError(f"❌ Direct GPIO - {gpio_peripheral} Pin {pin_number} did not reach state {expected_state} within {timeout}s. Last state: {final_state}")


    def wait_for_serial_hex_string(self, expected_hex_string, timeout=5.0):
        """Waits for a specific sequence of hexadecimal bytes on the COMMAND serial port.

        Args:
            expected_hex_string (str): The exact sequence of hex bytes to find,
                                       case-insensitive (e.g., "FD073800080000000047").
            timeout (float): Maximum time to wait in seconds. Defaults to 5.0.

        Returns:
            str: The found hex string (matched portion).

        Raises:
            AssertionError: If the hex string is not found within the timeout.
        """
        start_time = time.time()
        # Normalize expected string to uppercase for consistent comparison
        search_string_upper = expected_hex_string.upper()
        # Use bytearray to efficiently store and search raw bytes
        byte_buffer = bytearray()

        print(f"Waiting for serial HEX string: {search_string_upper}") # Debug

        # Check initial buffer if any exists
        try:
            if self.serial_port_cmd and self.serial_port_cmd.is_open and self.serial_port_cmd.in_waiting > 0:
                 initial_bytes = self.serial_port_cmd.read(self.serial_port_cmd.in_waiting)
                 byte_buffer.extend(initial_bytes)
                 # Also add decoded version to main log for context in case of failure
                 self._serial_cmd_data += initial_bytes.decode('utf-8', errors='replace')
                 # Initial check
                 current_hex_repr = ''.join(f'{b:02X}' for b in byte_buffer)
                 if search_string_upper in current_hex_repr:
                     print(f"✅ Found HEX string in initial buffer: {search_string_upper}")
                     return search_string_upper
        except (serial.SerialException, OSError) as e:
             print(f"Warning: Serial exception checking initial buffer: {e}")
             # Handle port reopening if necessary...

        # Main loop reading new data
        while time.time() - start_time < timeout:
            try:
                if self.serial_port_cmd and self.serial_port_cmd.is_open and self.serial_port_cmd.in_waiting > 0:
                    new_data_bytes = self.serial_port_cmd.read(self.serial_port_cmd.in_waiting)
                          
                    if new_data_bytes:

                        hex_representation = ' '.join(f'{b:02X}' for b in new_data_bytes)
                        # print(f"{hex_representation.replace('FD', '\nFD')}") # Keep this for debugging

                        byte_buffer.extend(new_data_bytes)
                        # Also add decoded version to main log
                        self._serial_cmd_data += new_data_bytes.decode('utf-8', errors='replace')

                        # <<< Search the HEX representation of the entire buffer >>>
                        current_hex_repr = ''.join(f'{b:02X}' for b in byte_buffer)
                        if search_string_upper in current_hex_repr:
                            print(f"✅ Found HEX string: {search_string_upper}")
                            return search_string_upper # Return the string we were looking for

    

            except (serial.SerialException, OSError) as e:
                 print(f"Warning: Serial exception during read: {e}")
                 # Handle port reopening if necessary...
                 if not self.serial_port_cmd or not self.serial_port_cmd.is_open:
                     raise AssertionError(f"❌ Serial port disconnected during wait.")

            time.sleep(0.01) # Small sleep

        # Timeout handling: Check the final buffer one last time
        print(f"Timeout reached. Checking final HEX buffer ({len(byte_buffer)} bytes) for string: {search_string_upper}")
        final_hex_repr = ''.join(f'{b:02X}' for b in byte_buffer)
        if search_string_upper in final_hex_repr:
             print(f"✅ Found HEX string in final buffer: {search_string_upper}")
             return search_string_upper

        # If not found after timeout
        raise AssertionError(f"❌ Failed to find serial HEX string: '{expected_hex_string}' within {timeout}s. Full HEX Buffer:\n{final_hex_repr.replace('FD', '\nFD')}")


    # --- Cleanup ---
    def _close(self):
        """Closes CAN, serial ports, and telnet connection."""
        # ... (existing close_serial and close_can calls) ...
        if self.serial_port_cmd is not None:
            try:
                if self.serial_port_cmd.is_open:
                    self.serial_port_cmd.close()
                self.serial_port_cmd = None
            except Exception as e:
                print(f"Error closing command serial port: {e}")

        self.disconnect_from_renode_monitor() # NEW: Disconnect telnet

        if self.can_bus is not None:
            try:
                self.can_bus.shutdown()
                self.can_bus = None
            except Exception as e:
                print(f"Error shutting down CAN bus: {e}")

    def __del__(self):
        self._close()

    # --- Other keywords (stop_all_logging) remain the same ---
    def stop_all_logging(self):
        """
        Stops the logging threads.
        """
        global running  # Access the global running flag
        running = False