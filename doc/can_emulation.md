# Emulating CAN Traffic with Renode and `can-utils`

This document describes how to emulate CAN bus traffic in a Renode simulation and interact with it using the `can-utils` suite on a Linux host. This allows you to test your firmware's CAN communication logic without needing real CAN hardware. We'll cover:

1.  **Setting up a Virtual CAN Interface (`vcan0`)**:  Creating the virtual CAN interface on your Linux host.
2.  **Configuring Renode to Connect to `vcan0`**:  Modifying your Renode platform (`.repl`) file to connect the simulated CAN controller to the `vcan0` interface.
3.  **Sending CAN Frames with `cansend`**:  Using the `cansend` utility to inject CAN messages into the simulation.
4.  **Receiving CAN Frames with `candump`**:  Using the `candump` utility to monitor CAN traffic sent by the simulated firmware.
5.  **Using `canplayer` with CSV Log Files**:  Replaying pre-recorded CAN traffic using `canplayer` and a CSV log file.
6. **Important notes**

**Prerequisites:**

*   Renode installed and working.
*   A compiled firmware image (`.elf` file) for your target microcontroller (e.g., STM32F103) that includes CAN communication logic.
*   The `can-utils` package installed on your Linux host (`sudo apt-get install can-utils`, `sudo dnf install can-utils`, etc.).
*   A Renode platform definition file (`.repl`) that correctly models your target hardware, including the CAN controller. The examples will use the STM32F103, as in previous discussions.

## 1. Setting up `vcan0`

The `vcan` module provides virtual CAN interfaces.  These are software-only interfaces, perfect for testing without physical CAN hardware.

```bash
sudo modprobe vcan  # Load the vcan kernel module (if not already loaded)
sudo ip link add dev vcan0 type vcan
sudo ip link set vcan0 mtu 16
sudo ip link set up vcan0
```

Verify that `vcan0` is up:

```bash
ip link show vcan0
```

You should see output like this:

```
4: vcan0: <NOARP,UP,LOWER_UP,ECHO> mtu 72 qdisc noqueue state UNKNOWN mode DEFAULT group default qlen 1000
    link/can
```

To remove `vcan0` later:

```bash
sudo ip link set down vcan0
sudo ip link delete dev vcan0
```

## 2. Configuring Renode to Connect to `vcan0`

Modify your Renode platform description file (`.repl`, e.g., `volvo_od2.repl`) to connect the simulated CAN controller to the `vcan0` interface.

**Create renode CAN for STM32F103 using the following repl file:**

```repl
can1: CAN.STMCAN @ sysbus <0x40006400, +0x400>
    [0-3] -> nvic@[19-22]

socketcan: CAN.SocketCANBridge @ sysbus
    canInterfaceName: "vcan0"
    ensureFdFrames: false
    ensureXlFrames: false
```

**Key elements:**

*   **`can1: CAN.STMCAN @ sysbus 0x40006400`:**  This defines the CAN controller peripheral.  The `CAN.STMCAN` is Renode's built-in model for the STM32's CAN controller.  The address `0x40006400` is the correct base address for CAN1 on the STM32F103. The IRQ 19 to 22 is used based on STM32 Vector table.


Your `.resc` file will typically look something like this:

```repl
# volvo_od2.resc

using sysbus
$name?="volvo_od2"
mach create $name

# Load the platform definition.
machine LoadPlatformDescription @volvo_od2.repl

# Load the firmware.
sysbus LoadELF $ORIGIN/../qemu/firmware.elf  # Adjust path to your ELF

emulation CreateCANHub "canHub"

# Connect CAN controller to the CAN hub
connector Connect sysbus.can1 canHub

# Connect bridge to the internal network
connector Connect socketcan canHub

start
```

## 3. Sending CAN Frames with `cansend`

The `cansend` utility (part of `can-utils`) sends individual CAN frames.

**Basic Syntax:**

```bash
cansend <interface> <can_id>#{data}
```

*   **`<interface>`:**  The CAN interface (e.g., `vcan0`).
*   **`<can_id>`:**  The CAN ID (in hexadecimal).  Can be 11-bit (standard) or 29-bit (extended).
*   **`#`:** Separator between ID and data.
*   **`{data}`:** The data payload (in hexadecimal), up to 8 bytes.

**Examples:**

*   **Standard ID (11-bit):**

    ```bash
    cansend vcan0 123#DEADC0DE
    ```

    This sends a CAN frame with ID `0x123` and data `DE AD C0 DE` (4 bytes).

*   **Extended ID (29-bit):**

    ```bash
    cansend vcan0 18FEDF00#AABBCCDD
    ```

    This sends a CAN frame with extended ID `0x18FEDF00` and data `AA BB CC DD`.

*   **No Data:**

    ```bash
    cansend vcan0 5A1#
    ```
    Sends a CAN frame with ID `0x5A1` and *no* data.

*   **Remote Frame (RTR):**  To send a *remote frame* (a request for data), use the `-r` option and specify the *length* of the data you're requesting:

    ```bash
    cansend vcan0 -r 4 7FF#  # Request 4 bytes of data from ID 0x7FF
    ```

**Important Notes on `cansend`:**

*   **Hexadecimal:**  Both the ID and data *must* be in hexadecimal format.
*   **Data Length:** The number of data bytes must be between 0 and 8, inclusive.  If you specify more than 8 bytes, `cansend` will truncate the data.
*   **Running Renode:**  Your Renode simulation *must* be running *before* you use `cansend`.  Otherwise, the messages will be sent to `vcan0`, but nothing will be there to receive them.

## 4. Receiving CAN Frames with `candump`

The `candump` utility (also part of `can-utils`) displays CAN traffic on a specified interface.

**Basic Syntax:**

```bash
candump <interface>
```

*   **`<interface>`:** The CAN interface (e.g., `vcan0`).

**Examples:**

*   **Dump all traffic on `vcan0`:**

    ```bash
    candump vcan0
    ```

*   **Filtering:** `candump` supports filtering, which is *very* useful for isolating specific messages.  You can filter by ID, mask, and error flags.

    *   **Filter by ID:**

        ```bash
        candump vcan0,123:7FF  # Show only messages with ID 0x123
        ```

    *   **Filter by ID and Mask:**

        ```bash
        candump vcan0,100:7F0,200:7F0 # Show messages with ID 0x100-0x10F and 0x200-0x20F
        ```

    *   **Filter by Extended ID:**

        ```bash
         candump vcan0,18FEDF00:1FFFFFFF # Show only messages with extended ID 0x18FEDF00
        ```
    *    **Multiple filters:**
        ```bash
         candump vcan0,0x100:0x700,0x200:0x700 # Show messages with ID 0x100 or 0x200
        ```
*   **Timestamping:** By default, `candump` shows timestamps.  You can customize the timestamp format using options like `-t` (absolute time), `-T` (relative time), `-d` (delta time).  See the `candump` man page (`man candump`) for details.

*   **Logging:**  You can save the output of `candump` to a file using standard redirection:

    ```bash
    candump vcan0 > can_log.txt
    ```

**Example Workflow (Sending and Receiving):**

1.  **Start Renode:** Run your Renode simulation (which loads your firmware).
2.  **Open a terminal for `candump`:** `candump vcan0`
3.  **Open another terminal for `cansend`:** `cansend vcan0 123#DEADC0DE`
4.  **Observe:** You should see the message appear in the `candump` output, *and* your firmware (inside Renode) should receive the message (you can verify this with `printf` statements or GDB).

## 5. Using `canplayer` with CSV Log Files

`canplayer` is a powerful tool for *replaying* pre-recorded CAN traffic.  This is extremely useful for testing your firmware with realistic or complex scenarios.  While `candump` can output in various formats, a CSV log file provides a human-readable and easily editable format for creating test scenarios.

**5.1. Creating a CSV Log File**

A simple CSV file for `canplayer` has the following format:

```
timestamp,id,data
1678886400.000,0x123,DE AD C0 DE
1678886400.100,0x456,11 22 33 44
1678886400.250,0x123,AA BB CC DD
```

*   **`timestamp`:** The timestamp of the message, in seconds (can be fractional). This controls the *timing* of the replay.
*   **`id`:** The CAN ID (in hexadecimal, *without* the `0x` prefix).
*   **`data`:** The data payload (in hexadecimal, with spaces between bytes).

**Example `can_frames.csv`:**

```csv
timestamp,id,data
0.0,036,0E 00 06 2F 01 00 00 A0
0.1,123,01 02 03 04
0.2,456,0A 0B 0C 0D 0E 0F
0.5,036,0F 01 07 30 02 01 01 A1
1.0,789,00
1.1,123,05 06 07 08
```

**5.2. Using `canplayer`**

**Basic Syntax:**

```bash
canplayer -I <input_format> -i <interface> <logfile>
```

*   **`-I <input_format>`:**  Specifies the input file format.  For our CSV file, use `-I asc`. Important, there are other formats supported.
*   **`-i <interface>`:** The CAN interface (e.g., `vcan0`).
*   **`<logfile>`:** The path to your CSV log file (e.g., `can_frames.csv`).

**Example:**

```bash
canplayer -I asc -i vcan0 can_frames.csv
```

This command will:

1.  Read the `can_frames.csv` file.
2.  Parse each line, extracting the timestamp, ID, and data.
3.  Send the CAN frames to the `vcan0` interface, *respecting the timestamps* in the file.  This creates a realistic replay of the recorded traffic.

**Important `canplayer` Options:**

*   **`-l <count>` (Loop):**  Replays the log file `<count>` times.  `-l i` replays it *infinitely*.  This is very useful for continuous testing.
*   **`-t` (Timestamp Type):** There are two types of timestamps:
   *  **Absolute timestamp** `-t abs`: Timestamp value are interpreted as absolute time values
   * **Timestamp differences**: `-t self` (default): The timestamp will be interpreted as difference.
*   **`-g <gap>` (Gap):**  Specifies a time gap (in milliseconds) to wait *after* replaying the entire log file.  This is useful with `-l` to create periodic replays.
*   **`-v` (Verbose):** Prints more information about the replay process.
* **`-s` (speed):** Control speed, using a divider.

**Example (Infinite Loop with 1-second Gap):**

```bash
canplayer -I asc -i vcan0 -l i -g 1000 can_frames.csv
```

**5.3. Complete Renode Workflow with `canplayer`**

1.  **Create `can_frames.csv`:**  Create your CSV log file with the desired CAN messages and timestamps.

2.  **Setup `vcan0`:**
    ```bash
    sudo modprobe vcan
    sudo ip link add dev vcan0 type vcan
    sudo ip link set up vcan0
    ```

3.  **Start Renode:** `renode volvo_od2.resc` (using your `.repl` and `.resc` files).

4.  **Replay CAN Traffic:** In a *separate* terminal:

    ```bash
    canplayer -I asc -i vcan0 can_frames.csv
    ```

    (Add `-l i` and `-g` options as needed.)

5.  **Observe:**  Your firmware running inside Renode should now receive the CAN messages from the `canplayer` replay, just as if they were coming from a real CAN bus.  You can use Renode's debugging features (breakpoints, register inspection, UART output) to verify that your firmware is processing the messages correctly.

**Important Considerations**

* **`candump` Format:** The `candump` utility, *by default*, outputs in a format that is *not* directly compatible with `canplayer -I asc`.  You'll need to use the `-l` (lowercase L) with `candump`.

*   **Timestamp Accuracy:** Renode's timing is not *perfectly* real-time.  There will be some jitter and variation, especially if your host system is heavily loaded.  However, for most testing purposes, it's accurate enough.

*   **Complex Scenarios:** You can create very complex CAN traffic scenarios by carefully crafting your CSV log file. You can simulate different devices, error conditions, and varying message rates.
* **Extended ID**: You must declare if a message is extended adding a `E` before the message, like: `1678886400.100,E18FEDF00,AA BB CC DD`

This comprehensive guide covers all the steps needed to emulate CAN traffic using Renode and `can-utils`, including sending individual messages with `cansend`, monitoring traffic with `candump`, and replaying complex scenarios from CSV log files with `canplayer`. This setup provides a powerful and flexible environment for developing and testing your CAN-based embedded systems.
