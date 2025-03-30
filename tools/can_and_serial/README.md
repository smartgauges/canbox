# Logger for simultaneus loging of CAN and UART communication

We want to have a single synchronized log of both:
- CAN messages between CAR and CANBOX
- Serial commands/messages between CAR and HEADUNIT



*   **Complete Setup Functions:** `setup_can` and `setup_serial` now include all necessary steps (bringing the CAN interface up/down, setting baud rate, etc.).  They also return the created objects (or `None` on failure).
*   **Threaded Logging:**  The CAN bus and serial port are logged in *separate threads*.  This is *absolutely essential* to avoid blocking.  If you try to read from both in the same thread, whichever one you read *first* will block the other, leading to lost data.
*   **Error Handling:** The code includes `try...except` blocks to catch `can.CanError` and `serial.SerialException`. This makes the script more robust to problems with the CAN bus or serial port. Importantly, errors are only printed if the `running` flag is still True, avoiding spurious errors during shutdown.
*   **Timestamping:**  Both CAN and serial data are timestamped using `message.timestamp` (for CAN, very precise) and `time.time()` (for serial, less precise but sufficient).  Timestamps are in *seconds since the epoch* (Unix time), with fractional seconds for precision.
*   **CSV Output:** The log file is written in a simple CSV (Comma-Separated Values) format:
    *   `Type,Timestamp,Data`
    *   `Type` will be "CAN" or "SERIAL".
    *   `Timestamp` is the timestamp (seconds.fractional_seconds).
    *   `Data` depends on the type:
        *   **CAN:** `arbitration_id,dlc,data0,data1,data2,...` (hexadecimal)
        *   **SERIAL:** The raw line read from the serial port (UTF-8 decoded, with error replacement).
*   **`can-isotp` Library (Removed):**  The original request mentioned ISO-TP.  However, for *logging* raw CAN and serial data, you *don't* want or need the `can-isotp` library.  ISO-TP is a higher-level protocol *built on top of* CAN.  This script is designed for raw data capture, *before* any protocol decoding. The `can` library handles the low-level details of sending and receiving raw CAN frames.
*   **Global `running` Flag:**  A global `running` flag controls the logging threads. This allows for a clean shutdown on Ctrl+C.
*   **Shutdown Handling:** The `KeyboardInterrupt` exception (Ctrl+C) is caught.  The `running` flag is set to `False`, the threads are joined (waiting for them to finish), the CAN bus is shut down (VERY IMPORTANT), the serial port is closed, and the log file is closed.  This is *critical* for a clean exit.
*   **Line Buffering:** The log file is opened in line-buffered mode (`buffering=1`). This ensures that each line is written to the file immediately, which is important for real-time logging.
*   **Command-line Arguments:** The script now uses `argparse` to handle command-line arguments, making it much more flexible and user-friendly:
    *   `--can_interface`:  Specify the CAN interface (default: `can0`).
    *   `--can_type`: Specify the CAN interface type(default: socketcan)
    *   `--can_bitrate`: Specify CAN bitrate
    *   `--serial_port`: Specify the serial port (default: `/dev/ttyUSB0`).
    *   `--baud_rate`:  Specify the serial baud rate (default: `38400`).
    *   `--log_dir`: Specify the directory for log files (default: `can_logs`).
    *   `--log_prefix`: Specify a prefix for the log filenames (default: `can_serial_log`).
    *   `--no_can`: Disable CAN logging.
    *   `--no_serial`: Disable serial logging.
    *    `--canbox_debug_mode`:  Attempt to put the canbox into debug mode before starting logging.
*   **CAN Bus Setup:** The `setup_can` function now *correctly* sets up the `socketcan` interface using the `ip` command (which requires `sudo`). This is the standard way to configure SocketCAN on Linux. The bitrate is configurable.
* **Debug Mode:** Option `--canbox_debug_mode` added. If true, the script sends command to enable debug mode on canbox.
* **can.Bus Object:** The returned value is now correctly stored and used on `can_logger`.
* **Log format:** The script now saves log in plain text format instead of pcapng format.
* **Clear errors:** All identified errors in the user's script have been corrected.
* **File Handling:** File is opened in the `main()` function.

**How to Use (IMPORTANT):**

1.  **Save:** Save the script as a Python file (e.g., `logger.py`).
2.  **Permissions:** Make the script executable: `chmod +x logger.py`
3.  **Run with `sudo`:**  You *must* run the script with `sudo` privileges to configure the CAN interface:

    ```bash
    sudo ./logger.py
    ```

    Or, to specify options:

    ```bash
    sudo ./logger.py --can_interface can0 --serial_port /dev/ttyUSB0 --baud_rate 38400 --canbox_debug_mode --log_dir my_logs
    ```
     This command will enable debug mode on canbox, and save log to `my_logs` directory.
    ```bash
     sudo ./logger.py --no_serial
    ```
    This command will disable serial port logging, so you log only CAN messages.
    ```bash
      sudo ./logger.py --no_can
     ```
     This command will disable CAN logging, so you log only canbox messages.
4.  **Stop with Ctrl+C:** Press Ctrl+C in the terminal to stop logging. The script will cleanly shut down the CAN interface and close the files.

**To configure Canbox for headunit communication, follow these steps:**

1.  **Connect:** Connect your USB-to-serial adapter to your computer and the CAN box's RX/TX lines.  *Crucially*, power the CAN box from the car or OBD2 connector, *not* from the USB-to-serial adapter's VCC (unless you are *absolutely certain* about the voltage levels).
2.  **Run the script:** Run the script *with* the `--canbox_debug_mode` flag:

    ```bash
    sudo ./logger.py --canbox_debug_mode
    ```

3.  **Observe the Output:**  The script will attempt to put the CAN box into debug mode by sending the `OOOOOOOOOOOO` sequence. You should see some output from the CAN box in the terminal (if it successfully entered debug mode).  The script *will* proceed to logging even if debug mode entry fails – this just attempts the initial setup.
4.  **Configure (in Debug Mode):** If you see the debug output, you can now send commands to configure the CAN box.  As described in the `canbox` project's documentation (and the `DRIVE2.md` file), use these commands:
    *   `o`: Exit debug mode.
    *   `b`: Select emulated CANBUS protocol.  You'll likely want to try `Raise VW(PQ)`, `Raise VW(MQB)`, `HiWorld VW(MQB)`, etc., and see which one works best with your headunit.
    *   `c`: Select car model. Choose `Any Msg` initially for reverse engineering.
    *   `s`: Save configuration (important!).
    *   `S`: Switch to CAN Sniffer mode.
    *   `I/i`: Increase/Decrease Backlight threshold.
    *   `D/d`: Increase/Decrease the rear camera turn-off delay.
    *   `m`: Select next can ID to show on main screen in debug mode.

    For example, to set the protocol to "Raise VW(PQ)" and save it, you would type `b` (and likely cycle through the options with repeated presses of `b` until you see "Raise VW(PQ)" displayed), then type `s` and press Enter.

5.  **Test:**  After configuring, exit debug mode (`o`), interact with your car and head unit (turn the ignition on/off, press buttons, shift gears, etc.), and observe the logged data to see if the head unit responds correctly.

This improved script and the detailed instructions give you a powerful tool for capturing the necessary data to reverse engineer the communication between your Peugeot 407 and its head unit.  Remember to combine this with the CAN message definitions from the simulator project and careful observation of the data.
