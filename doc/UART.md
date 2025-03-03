# UART Debugging and Logging for CAN Box Firmware

This document provides detailed instructions on how to connect to the CAN box via its UART interface (RX/TX lines) from a Linux machine, enable debug mode, log the communication between the CAN box and the Android head unit, and interpret the output.  This is crucial for debugging, reverse engineering the communication protocol, and customizing the firmware.

## Hardware Setup

### 1. USB-to-Serial Adapter

You'll need a USB-to-serial adapter that supports **3.3V logic levels**.  Using a 5V adapter can *damage* the CAN box's microcontroller.  Common, reliable adapters use chips like:

*   **FTDI FT232R or FT231X:** These are very popular and generally well-supported on Linux.
*   **CP210x (Silicon Labs):**  Also a good option, often found in slightly less expensive adapters.
*   **CH340/CH341:**  Common in inexpensive adapters.  They usually work, but driver support might require a bit more setup on some Linux distributions.

**Important:**  *Before* connecting the adapter to the CAN box, use a multimeter to verify that the TX pin on the adapter is outputting 3.3V (or very close to it) relative to the adapter's GND pin.

### 2. Wiring

Connect the adapter to the CAN box as follows:

| Adapter Pin | CAN Box Pin |
| :---------- | :---------- |
| TX          | RX          |
| RX          | TX          |
| GND         | GND         |

*   **TX (Transmit):**  The adapter's transmit pin sends data *to* the CAN box's receive pin.
*   **RX (Receive):** The adapter's receive pin receives data *from* the CAN box's transmit pin.
*   **GND (Ground):**  *Essential* for a common ground reference.  Without a common ground, communication will not work, and you could damage the hardware.

**Do *not* connect the adapter's VCC (power) pin to the CAN box unless you are absolutely certain about the voltage levels and power requirements.**  It's safer to power the CAN box through its normal vehicle power connection (or the OBD2 connector).

### 3. OBD2 Connector (Recommended)

Using the OBD2 connector for power and CAN bus access is a convenient and safe way to test and debug.  This eliminates the need to tap into the vehicle's wiring harness during development.

## Software Setup (Linux)

### 1. Identify the Serial Port

After plugging in the USB-to-serial adapter, Linux will create a device file.  Common device file names are:

*   `/dev/ttyUSB0`
*   `/dev/ttyUSB1`
*   `/dev/ttyACM0`
*   `/dev/ttyACM1`

To find the correct device:

*   **Method 1: `dmesg` (Recommended):**

    1.  Open a terminal.
    2.  Plug in the USB-to-serial adapter.
    3.  Immediately run the command: `dmesg | grep tty`
    4.  Look for messages similar to this:

        ```
        [12345.67890] usb 1-1: FTDI USB Serial Device converter now attached to ttyUSB0
        ```

        This tells you the device is `/dev/ttyUSB0`.

*   **Method 2: Listing `/dev/tty*`:**

    1.  Open a terminal.
    2.  Run `ls /dev/tty*` *before* plugging in the adapter.  Note the output.
    3.  Plug in the adapter.
    4.  Run `ls /dev/tty*` again.  The new device file is the one you need.

*   **Method 3: `udevadm` (More Advanced):**

    This method is more robust, as it identifies the device based on its USB properties, not just the order it was plugged in.

    ```bash
    udevadm monitor --kernel --property --subsystem-match=tty
    ```
    Plug in the adapter. You will see udev events, including the device path (e.g., `/dev/ttyUSB0`).  Press Ctrl+C to stop `udevadm`. Look for lines that mention "tty" and "add".

### 2. Install a Terminal Program

You need a terminal program to communicate with the serial port.  Here are some good options (choose one):

*   **`minicom` (full-featured, text-based):**

    ```bash
    sudo apt install minicom  # Or your distribution's package manager
    ```

*   **`screen` (simple, often pre-installed):**

    ```bash
    # Usually pre-installed, no need to install
    ```

*   **`picocom` (lightweight, easy to use):**

    ```bash
    sudo apt install picocom  # Or your distribution's package manager
    ```
* **`tio` (modern and user friendly)**
    ```bash
     sudo apt install tio
    ```

*   **GTKTerm (graphical):**

    ```bash
    sudo apt install gtkterm  # Or your distribution's package manager
    ```

*   **CuteCom (graphical):**

    ```bash
    sudo apt install cutecom  # Or your distribution's package manager
    ```

### 3. Configure and Run the Terminal Program

The CAN box communicates at a baud rate of **38400**, with **8 data bits**, **no parity**, and **1 stop bit** (8N1).

*   **`minicom` (Configuration):**

    1.  Run `sudo minicom -s`.  (You might need `sudo` for initial setup, and potentially for every use, depending on permissions.)
    2.  Navigate to "Serial port setup" using the arrow keys and press Enter.
    3.  Press `A` and change the "Serial Device" to the correct device (e.g., `/dev/ttyUSB0`).  Press Enter.
    4.  Press `E` and set the "Bps/Par/Bits" to "38400 8N1".  Use the menu to select the correct values. Press Enter.
    5.  Press `F` to set "Hardware Flow Control" to "No".
    6.  Press `G` to set "Software Flow Control" to "No".
    7.  Press Enter to exit the serial port setup menu.
    8.  Select "Save setup as dfl" to save these settings as the default.
    9.  Select "Exit" to start the terminal session.

    *   To exit `minicom`, press `Ctrl+A`, then `X`, then confirm with Enter.

*   **`screen` (Simple, but limited):**

    ```bash
    screen /dev/ttyUSB0 38400
    ```
    *   To exit `screen`, press `Ctrl+a`, then `k`, then `y`.  This *kills* the screen session.  There's no built-in logging.

*   **`picocom` (Recommended for Simplicity):**

    ```bash
    picocom -b 38400 /dev/ttyUSB0
    ```

    *   To exit `picocom`, press `Ctrl+a`, then `Ctrl+x`.

*   **`tio` (Recommended for Simplicity):**
    ```bash
    tio -b 38400 /dev/ttyUSB0
    ```
   *   To exit tio, press Ctrl+t, then q.

*   **GTKTerm or CuteCom:**
    *   Launch the program from your desktop environment's menu.
    *   Configure the serial port settings (port, baud rate, data bits, parity, stop bits, flow control) through the GUI.

### 4. Enable Debug Mode

Once the terminal program is connected and running, you need to put the CAN box into debug mode.  Do this by sending the following sequence of characters *quickly* (within 1 second):

```
OOOOOOOOOOOO
```

You should see debug output appear in the terminal. If you don't, double-check your wiring and serial port settings.  If it's still not working, try disconnecting and reconnecting the USB-to-serial adapter.

### 5. Logging Output

**Using `minicom`'s Capture Feature (Recommended):**

1.  **Before** connecting to the CAN box (i.e., before running `minicom`), start the capture: Press `Ctrl+A`, then `L`.
2.  Enter a filename for the log (e.g., `canbox_log.txt`).
3.  Connect to the CAN box using `minicom /dev/ttyUSB0 -b 38400`.  The `-b 38400` sets the baud rate.
4.  Enter debug mode (`OOOOOOOOOOOO`).
5.  Interact with the car and head unit.
6.  When finished, press `Ctrl+A`, then `L` again to stop capture.
7.  Exit `minicom` with `Ctrl+A`, then `X`.

**Using Shell Redirection (Works with any terminal program):**

```bash
picocom -b 38400 /dev/ttyUSB0 > can_log.txt 2>&1 &
```
Or, with `screen` (less recommended, but works):
```bash
screen /dev/ttyUSB0 38400 > can_log.txt 2>&1
```
**Important:** With shell redirection, the log file will contain *everything* sent to the terminal, including your commands to enter debug mode.

### 6. Interpreting the Output

The debug output will show you:

*   **CAN messages received from the car:** These will be displayed in a hexadecimal format, along with their ID.
*   **Decoded data:** The `car.c` file decodes the raw CAN data and stores it in the `carstate` structure.  The debug output will show you the values of these decoded variables (e.g., ignition status, speed, door status).
*   **Messages sent to the head unit:** The `canbox.c` file formats the vehicle data into messages that the head unit understands.  The debug output will show you the raw bytes of these messages.
*   **Configuration settings:** The debug output will show the current configuration of the CAN box (e.g., selected car model, CAN bus speed).

By analyzing this output, you can:

*   Verify that the CAN box is receiving data from the car's CAN bus.
*   Confirm that the CAN message IDs and data formats in `cars/peugeot_407.c` are correct.
*   See how the CAN box is translating the raw CAN data into the format expected by the head unit.
*   Identify any discrepancies between the expected and actual behavior.

### Example Debug Output

The `README.md` file provides an example of the debug output:

```
Main window
--------------------
Configuration(63)
--------------------
Car: Any Msg Vin: na  0 km
CanBox: Raise VW(PQ)
Conf: Illum:50 Rear Delay:1500
State
--------------------
Uptime: 00003.770 Wakeups: 0 Wheel:0
Acc:1 Ign:1 Selector:x R:0
Illum:0 ParkLights:0 NearLights:0
Park:ff Front:00 00 00 00 Rear:00 00 00 00
Can: IDs:3 Msgs:18 Irqs:0
Can1/3:  0000007e:00.00.98.07.d0.00.80.00
--------------------
Ctrl keys: o - exit; m - shift msgs
I/i - set Illum; d/D - set rear Delay
c - set Car; b - set Canbox
s - save conf; S - to Sniffer window
```

This shows:

*   **Configuration:** The selected car model, CAN box type, illumination threshold, and rear camera delay.
*   **State:**  The uptime, ignition status, gear selector, wheel angle, light status, and parking sensor data (if applicable).
*   **CAN:**  The number of unique CAN IDs received, the total number of messages, and the number of CAN interrupts.
*   **CanX/Y:** The raw data from a specific CAN message (ID and data bytes).
*   **Ctrl keys:** The commands you can use in debug mode.

### Troubleshooting
If you're seeing errors, double check these points:
*   **Correct Port:** Ensure `/dev/ttyUSB0` (or whichever port) is the correct one.
*   **Permissions:** You might need to add your user to the `dialout` group (or a similar group) to have permission to access the serial port. `sudo usermod -a -G dialout $USER` and then log out and back in.
*   **Baud Rate:** 38400 is correct.
*   **Wiring:** TX/RX swapped? GND connected?  Voltage levels correct (3.3V)?
*   **CAN Box Power:** The CAN box must be powered.  Using the OBD2 connector as you've described is a good way to ensure this.
* **Firmware**: Recompile your firmware with added `#define debug 1` in `conf.c`

By carefully examining the debug output and comparing it to the expected CAN data for your vehicle, you can identify and fix any problems in the firmware.
