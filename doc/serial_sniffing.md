# Sniffing Serial Communication between Canbox and Headunit on openSUSE Tumbleweed

This document outlines how to sniff the serial communication between a `canbox` device and an aftermarket headunit on an openSUSE Tumbleweed system.  We'll focus on using a *single* USB-to-serial adapter, capturing data in two separate passes: first, capturing data *from* the `canbox`, and second, capturing data *from* the headunit. We'll also cover how to save the captured communication to a file.

## Table of Contents

1.  [Introduction](#introduction)
2.  [Method: Using a Single USB-to-Serial Adapter](#method-using-a-single-usb-to-serial-adapter)
    *   [Hardware Required](#hardware-required)
    *   [Software Required](#software-required)
    *   [Connections (Canbox -> Headunit)](#connections-canbox---headunit)
    *   [Procedure (Canbox -> Headunit)](#procedure-canbox---headunit)
    *   [Connections (Headunit -> Canbox)](#connections-headunit---canbox)
    *   [Procedure (Headunit -> Canbox)](#procedure-headunit---canbox)
3.  [Saving Serial Data to a File](#saving-serial-data-to-a-file)
    *   [Using `minicom`'s Capture Feature](#using-minicoms-capture-feature)
    *   [Using `picocom` with Redirection](#using-picocom-with-redirection)
    *    [Using `tio` with redirection](#using-tio-with-redirection)
    *   [Using `screen` (Advanced)](#using-screen-advanced)
4.  [Troubleshooting](#troubleshooting)

## 1. Introduction <a name="introduction"></a>

This guide provides a practical approach to capturing serial data in scenarios where only one USB-to-serial adapter is available.  By performing two separate captures, we can analyze the bidirectional communication.  This method avoids potential conflicts that could arise from trying to simultaneously transmit and receive on the same adapter.

## 2. Method: Using a Single USB-to-Serial Adapter <a name="method-using-a-single-usb-to-serial-adapter"></a>

This method uses a single USB-to-serial adapter to capture the communication in two separate passes.

### Hardware Required <a name="hardware-required"></a>

*   **One USB-to-Serial Adapter:**  Ensure it supports the correct voltage levels (likely 3.3V for `canbox`; check your headunit's documentation). CP2102, FT232RL, or CH340G-based adapters are common choices.
*   **Jumper Wires:**  Female-to-female jumper wires.
*   **Computer running openSUSE Tumbleweed:** With the necessary drivers for the USB-to-serial adapter.

### Software Required <a name="software-required"></a>

*   **Terminal Emulator:** Choose one of the following:
    *   **`minicom`:** (Recommended for its capture feature)
    *   **`picocom`:** (Lightweight and good for redirection)
    *   **`tio`:** (Simple and easy to use)
    *   **`screen`:** (Advanced users only)

    Install:

    ```bash
    sudo zypper install minicom  # Or picocom, or tio
    ```

### Connections (Canbox -> Headunit) <a name="connections-canbox---headunit"></a>

This connection setup is for capturing data transmitted *from* the `canbox` *to* the headunit.

| `canbox` | Headunit | USB-to-Serial Adapter | Notes                                     |
| :------- | :------- | :-------------------- | :---------------------------------------- |
| TX       | RX       | RX                    | `canbox` transmits, adapter listens.     |
| RX       | TX       | *NOT CONNECTED*       | *Do not connect* the headunit's TX.       |
| GND      | GND      | GND                   | **Essential:** Connect all grounds.       |

**Connection Diagram (Canbox -> Headunit):**

```
                +---------------------+       +---------------------+
                |     canbox          |       |      Headunit       |
                |                     |       |                     |
                |  TX  -----> RX      |       |  RX  <----- TX      |
                |  RX       TX       |       |  TX       RX       |
                | GND ----- GND      |       | GND ----- GND      |
                +--------|------------+       +--------|------------+
                         |                             |
                         +-------> RX                  |
                                 |                     |
                        +--------|------------+       |
                        |  USB-to-Serial    |       |
                        |     Adapter      |       |
                        |                     |       |
                        | *TX (NOT CONNECTED)*|       |
                        | GND ----- GND      |       |
                        +---------------------+       |
                                 ^                    |
                                 |                    |
                             Computer                 |
                                                      |
                                                      |
                                                      |
```

### Procedure (Canbox -> Headunit) <a name="procedure-canbox---headunit"></a>

1.  **Identify Serial Port:** Connect the USB-to-serial adapter.  Use `dmesg | tail` to find the device name (e.g., `/dev/ttyUSB0`).

2.  **Configure `minicom` (or your chosen terminal):**

    ```bash
    sudo minicom -s -c on
    ```

    *   "Serial port setup":
        *   "Serial Device":  `/dev/ttyUSB0` (or your device name).
        *   "Bps/Par/Bits":  Match the `canbox`/headunit baud rate (e.g., 115200), 8N1.
        *   "Hardware Flow Control": `No`.
        *   "Software Flow Control": `No`.
    *   "Save setup as canbox-tx" (or a descriptive name).
    *   "Exit from Minicom".

3.  **Connect Hardware:**  Make the connections as shown in the "Connections (Canbox -> Headunit)" table *above*. **Double-check before powering on.**

4.  **Start Capture:**

    ```bash
    minicom -c on canbox-tx
    ```

    *   Press `Ctrl-A` then `L` to start capturing.  `minicom` will prompt for a filename (e.g., `canbox_tx.log`).
    *   Perform the actions on the `canbox` and/or car that you want to capture (e.g., turn the ignition on/off, press steering wheel buttons).
    *   Press `Ctrl-A` then `L` again to stop capturing.

### Connections (Headunit -> Canbox) <a name="connections-headunit---canbox"></a>

This connection setup is for capturing data transmitted *from* the headunit *to* the `canbox`.

| Headunit | `canbox` | USB-to-Serial Adapter | Notes                                     |
| :------- | :------- | :-------------------- | :---------------------------------------- |
| TX       | RX       | RX                    | Headunit transmits, adapter listens.      |
| RX       | TX       | *NOT CONNECTED*       | *Do not connect* the `canbox`'s TX.       |
| GND      | GND      | GND                   | **Essential:** Connect all grounds.       |

**Connection Diagram (Headunit -> Canbox):**
```
                +---------------------+       +---------------------+
                |     canbox          |       |      Headunit       |
                |                     |       |                     |
                |  TX       RX      |       |  RX       TX      |
                |  RX  <----- TX      |       |  TX  -----> RX       |
                | GND ----- GND      |       | GND ----- GND      |
                +--------|------------+       +--------|------------+
                         |                             |
                         |             RX <-------------+
                         |                             |
                +--------|------------+               |
                |  USB-to-Serial    |               |
                |     Adapter      |               |
                |                     |               |
                | *TX (NOT CONNECTED)*|               |
                | GND ----- GND      |               |
                +---------------------+               |
                          ^                           |
                          |                           |
                      Computer                        |
```

### Procedure (Headunit -> Canbox) <a name="procedure-headunit---canbox"></a>

1.  **Configure `minicom` (or chosen terminal):**  You can use the same configuration as before, but it's good practice to save a separate one:

    ```bash
    sudo minicom -s -c on
    ```
    * Serial Device is the same
    *   "Save setup as headunit-tx" (or a descriptive name).
    *   "Exit from Minicom".

2.  **Connect Hardware:** Make the connections as shown in the "Connections (Headunit -> Canbox)" table.

3.  **Start Capture:**

    ```bash
    minicom -c on headunit-tx
    ```

    *   Press `Ctrl-A` then `L` to start capturing.  Enter a filename (e.g., `headunit_tx.log`).
    *   Perform actions on the headunit that you want to capture (e.g., press buttons, change settings).
    *   Press `Ctrl-A` then `L` again to stop capturing.

## 3. Saving Serial Data to a File <a name="saving-serial-data-to-a-file"></a>

### Using `minicom`'s Capture Feature <a name="using-minicoms-capture-feature"></a>

As described above, use `Ctrl-A` then `L` to start and stop capture within `minicom`.  This is the recommended method when using `minicom`.

### Using `picocom` with Redirection <a name="using-picocom-with-redirection"></a>

`picocom` doesn't have a built-in capture feature, but you can easily redirect its output to a file using standard shell redirection:

```bash
sudo picocom -b <baudrate> /dev/ttyUSBx > output.log 2>&1
```

*   `-b <baudrate>`:  Specifies the baud rate.
*   `/dev/ttyUSBx`: Your serial port.
*   `> output.log`:  Redirects standard output to `output.log`.
*   `2>&1`: Redirects standard error (stderr) to the same place as standard output (stdout), ensuring *all* output goes to the file.

To stop capturing, press `Ctrl-A` then `Ctrl-Q`.

### Using `tio` with Redirection <a name="using-tio-with-redirection"></a>
`tio` also support output to file by redirection:
```bash
sudo tio -b <baudrate> /dev/ttyUSBx > output.log 2>&1
```

*   `-b <baudrate>`:  Specifies the baud rate.
*   `/dev/ttyUSBx`: Your serial port.
*   `> output.log`:  Redirects standard output to `output.log`.
*   `2>&1`: Redirects standard error (stderr) to the same place as standard output (stdout), ensuring *all* output goes to the file.

To stop capturing, press `Ctrl-T` then `q`.

### Using `screen` (Advanced) <a name="using-screen-advanced"></a>

`screen` is primarily a terminal multiplexer, but it can also capture serial data. This is less intuitive than `minicom` or `picocom` for this specific task, but it's included for completeness.

1.  **Start `screen` with logging enabled:**

    ```bash
    sudo screen -L -Logfile serial.log /dev/ttyUSBx <baudrate>
    ```

    *   `-L`: Enables logging.
    *   `-Logfile serial.log`: Specifies the log file.
    *   `/dev/ttyUSBx`: Your serial port.
    *   `<baudrate>`: The baud rate.

2.  **Detach from `screen` (Optional):**  You can detach from the `screen` session (leaving it running in the background) by pressing `Ctrl-A` then `d`.

3.  **Reattach to `screen` (Optional):**  To reattach to the session later, use:

    ```bash
    screen -r
    ```

4.  **Stop Capturing and Exit `screen`:**  To stop capturing and exit `screen`, you need to be *attached* to the session. Then, press `Ctrl-A` then `k`, and confirm with `y`.  The captured data will be in `serial.log`.

## 4. Troubleshooting <a name="troubleshooting"></a>

*   **No Data:**
    *   Double-check connections, especially ground.
    *   Verify baud rate, data bits, parity, and stop bits.
    *   Ensure the correct serial port is selected.
    *   Try swapping RX/TX on the adapter (but *only* for listening, *never* connect TX when sniffing).
    * Check devices voltage levels.
*   **Garbled Data:**
    *   Incorrect baud rate is the most likely cause.
    *   Check for noise.
    *   Ensure good ground.
*   **`minicom` Issues:**
    *   Device locked: Make sure no other program is using the port (`lsof /dev/ttyUSBx`).
    *   Permission issues: Add your user to the `dialout` or `uucp` group:
        ```bash
        sudo usermod -a -G dialout $USER
        sudo usermod -a -G uucp $USER
        ```
        Log out and back in for group changes to take effect.

This comprehensive guide provides the steps to sniff serial communication using a single adapter on openSUSE Tumbleweed, including saving the data to a file for later analysis.  The two-pass approach ensures reliable capture without interference. Using `minicom` with its built-in capture feature, or `picocom`/`tio` with redirection, are the recommended methods.


