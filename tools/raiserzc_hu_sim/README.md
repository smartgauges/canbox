# RZC PSA CANbox Head Unit Simulator (`raiserzc_hu_sim.py`)

## 1. Overview

This Python script simulates a basic aftermarket Android head unit receiving data from a CAN bus decoder (CANbox) over a serial (UART) connection. Specifically, it is designed to parse and display data according to the **RZC (睿志诚) PSA protocol** detailed in `converted_document.md`.

This protocol is characterized by:
*   A `0xFD` header byte.
*   A specific frame structure (`FD LEN DTYPE DATA... CS`).
*   A baud rate of **19200 bps**.
*   An 8-bit additive checksum calculation (`SUM(LEN, DTYPE, DATA)`).

**Purpose:** This simulator is intended for **testing and debugging** the `canbox-renode` firmware when configured to output the RZC PSA protocol. It allows developers to see the vehicle state information being sent by the CANbox firmware in a human-readable format without needing a fully compatible physical head unit during development.

## 2. Features

*   Connects to a specified serial port.
*   Operates at the RZC protocol's standard **19200 baud rate** (configurable).
*   Parses incoming serial data streams to identify RZC frames starting with `0xFD`.
*   Validates frame length and checksum according to the RZC specification.
*   Decodes the payload based on the `DataType` byte for several key messages:
    *   `0x02`: Button Commands (Steering Wheel / Panel)
    *   `0x29`: Steering Wheel Angle (Displays raw value)
    *   `0x32`: Reverse Radar Information (Displays status and basic bar levels)
    *   `0x36`: Outside Temperature
    *   `0x38`: Vehicle Status (Doors, Lights, Park Brake, etc.)
    *   `0x33`, `0x34`, `0x35`: Trip Computer Pages (Inst. Cons, Range, Averages, Distance)
    *   `0x7F`: Version Information
    *   *(Add more DataTypes here as decoding is implemented)*
*   Maintains an internal dictionary representing the last known state of decoded vehicle parameters.
*   Displays the current vehicle state in a formatted table in the console.
*   Refreshes the display periodically and upon receiving new valid data.
*   Handles serial port errors and attempts reconnection.
*   Supports graceful shutdown via Ctrl+C.

## 3. Requirements

*   **Python 3** (3.6 or later recommended)
*   **PySerial** library: Used for serial port communication.

## 4. Installation

Install the PySerial library if you haven't already:

```bash
pip install pyserial
```

## 5. Usage

Run the script from your terminal, providing the serial port connected to your CAN box as a command-line argument.

**Basic Syntax:**

```bash
python raiserzc_hu_sim.py <serial_port> [options]
```

**Arguments:**

*   **`<serial_port>`:** (Required) The name of the serial port your CAN box is connected to.
    *   *Linux Example:* `/dev/ttyUSB0`, `/dev/ttyACM0`
    *   *Windows Example:* `COM3`, `COM4`
*   **`--baud <rate>` or `-b <rate>`:** (Optional) Specify the baud rate. Defaults to **19200** for the RZC protocol.

**Examples:**

1.  **Connect to `/dev/ttyUSB1` at the default 19200 baud:**
    ```bash
    python raiserzc_hu_sim.py /dev/ttyUSB1
    ```

2.  **Connect to `COM5` at 19200 baud:**
    ```bash
    python raiserzc_hu_sim.py COM5
    ```

3.  **Connect to `/dev/ttyUSB0` forcing 19200 baud (if default was changed):**
    ```bash
    python raiserzc_hu_sim.py /dev/ttyUSB0 --baud 19200
    ```

**Connection Note:** Ensure the **CAN box's TX pin** is connected to the **serial adapter's RX pin**, the **CAN box's RX pin** to the **adapter's TX pin**, and **GND** is connected between them. The `canbox` firmware should be configured and running, outputting the RZC PSA protocol at 19200 baud.

## 6. Output Format

The script will clear the console and continuously display the latest decoded vehicle state, similar to this example:

```text
--- RZC Head Unit Simulator ---
Timestamp: 2025-04-01 17:30:00
------------------------------
  Ignition : 1 | ACC: 1
  Lights   : Park(ON) Near(ON) | Illum: 15 (Enabled)
  Doors    : FR RL Trunk Open
  Status   : Gear(R) Brake(ON) Belt(OK)
  Steering : -250
  Speed    : 0 km/h | RPM: 850
  Odometer : 123456 km
  Temps    : Outside(15 C) Coolant(90 C) Oil(95 C)
  Fuel     : 33% | Low: OK
  Voltage  : 14.10 V | Low: OK
  Trip 0   : Inst:12.3L/100 Range:450km
  Trip 1   : AvgC:8.1L/100 AvgS:55km/h Dist:1207km
  Trip 2   : AvgC:7.5L/100 AvgS:62km/h Dist:398km
  Radar    : Status: Active+Disp | R:[----- ||||- |||--] F:[----- ----- -----]
  AC Info  : N/A
  Last Btn : Vol+ Pressed
------------------------------
Press Ctrl+C to exit.
```

*(Note: "N/A" indicates data for that `DataType` hasn't been received or decoded yet. Specific formatting depends on the implemented decoding functions.)*

## 7. Protocol Specifics

*   This script specifically targets the **RZC PSA protocol** documented in `converted_document.md`.
*   It expects frames starting with `0xFD` and uses the `SUM(LEN, DTYPE, DATA)` checksum.
*   The default baud rate is **19200 bps**.
*   It will **not** work correctly with other protocols like Hiworld (`0x5A 0xA5` header, 38400 baud) or standard Raise VW protocols (`0x2E` header, 38400 baud).

## 8. Limitations

*   **Simulator Only:** This script *simulates* receiving data; it does not replicate the full UI or logic of a real Android head unit.
*   **Decoding Completeness:** Only a subset of the DataTypes defined in the RZC specification are currently decoded. Contributions to add decoding for more DataTypes are welcome.
*   **No Commands Sent:** This script currently only listens. It does not send any commands (like `0x80` settings or `0xA6` time set) back to the CAN box.
*   **Assumptions:** Assumes the CAN box firmware correctly implements the RZC protocol transmission. Decoding accuracy depends on the correctness of the protocol specification document (`converted_document.md`) and the implementation in this script. **Real-world testing against the target head unit is always recommended.**

---