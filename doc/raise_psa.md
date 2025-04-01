## RZC PSA CANbox Protocol Documentation (for "Screen Removed" Installations)

### 1. Introduction

This document describes the serial communication protocol used by RZC (睿志诚科技) CANbus decoders (CANbox) designed for Peugeot/Citroën (PSA) vehicles in scenarios where the **original factory display screen is removed**. It outlines the communication rules between an aftermarket Android head unit (DVD 主机) and the RZC CANbox (协议盒). [https://github.com/cxsichen/helllo-world](https://github.com/cxsichen/helllo-world/blob/master/%E5%8D%8F%E8%AE%AE/%E7%9D%BF%E5%BF%97%E8%AF%9A/%E6%A0%87%E8%87%B4/%E6%A0%87%E8%87%B4%E9%9B%AA%E9%93%81%E9%BE%99%E6%8B%86%E5%8E%9F%E8%BD%A6%E5%B1%8F%E9%80%9A%E4%BF%A1%E5%8D%8F%E8%AE%AEV2.5.2.pdf)

This protocol allows the aftermarket head unit to receive vehicle status information (previously shown on the factory screen) and send control commands.

**Target Functionality (as implied by the protocol):**

*   Receive basic signals (ACC, Illumination, Reverse, Park - handled by CANbox hardware/firmware logic based on CAN).
*   Receive steering wheel and *potentially* panel button presses.
*   Receive climate control status.
*   Receive parking sensor data.
*   Receive trip computer data (Instantaneous, Trip 1, Trip 2).
*   Receive outside temperature.
*   Receive various vehicle status indicators (doors, lights, warnings).
*   Receive vehicle settings status.
*   *Potentially* send commands to configure vehicle settings (if supported by CANbox and vehicle).
*   *Potentially* send time synchronization commands.

### 2. Physical Layer

Communication uses a standard **UART** interface.

*   **Baud Rate:** **19200 bps**
*   **Data Bits:** 8
*   **Parity:** None
*   **Stop Bits:** 1
*   **Flow Control:** None
*   **Logic Level:** Assumed 3.3V (Verify with hardware).

### 3. Link Layer

**3.1 Frame Structure:**

Data is exchanged using frames with the following structure:

`HEADER (1 byte) | LENGTH (1 byte) | DATATYPE (1 byte) | DATA (n bytes) | CHECKSUM (1 byte)`

*   **HEADER:** Fixed value `0xFD`. Indicates the start of a frame.
*   **LENGTH (`LEN`):** 1 byte. Specifies the number of bytes *following* the `LENGTH` byte itself, up to *and including* the last `DATA` byte (`n`). It does **not** include the `HEADER` or `CHECKSUM`. Formula: `LEN = 1 (DataType byte) + n (Number of Data bytes)`.
*   **DATATYPE (`Zone`):** 1 byte. Command Identifier, analogous to `ComID`. Defines the purpose/content of the message.
*   **DATA:** `n` bytes (`Data0` to `Datan`). The actual payload, where `n = LEN - 1`. Structure depends on the `DataType`.
*   **CHECKSUM (`CS`):** 1 byte. Calculated as the **sum of `DataType`, `Length`, `Data0`, ... `Datan`**. The sum is performed using 8-bit arithmetic (modulo 256, overflows are ignored).

**3.2 Checksum Calculation Example:**

For a hypothetical frame sending DataType `0x02` (Button), Length `0x04` (1 byte Type + 3 bytes Data), Data `14 01 00`:

1.  Bytes to sum: `Length (0x04)`, `DataType (0x02)`, `Data0 (0x14)`, `Data1 (0x01)`, `Data2 (0x00)`
2.  Sum = `0x04 + 0x02 + 0x14 + 0x01 + 0x00 = 0x1B`
3.  Checksum = `0x1B`
4.  Full Frame: `FD 04 02 14 01 00 1B`

**3.3 Acknowledgement:**

The provided document **does not mention** any ACK (Acknowledge) or NACK (Not Acknowledge) mechanism for this `0xFD`-based protocol. Assume messages are sent without expecting a specific link-layer confirmation unless testing reveals otherwise.

### 4. Application Layer (DataType Definitions)

**Notation:**

*   **Slave:** CAN BUS Decoder (CANbox)
*   **Host:** DVD Navigation Host (Android Head Unit)
*   **Relevance:** Estimated relevance for the `canbox-renode` proxy project.

| DataType (Hex) | Direction     | Description                     | Relevance      | Priority     | Notes                                       |
| :------------- | :------------ | :------------------------------ | :------------- | :----------- | :------------------------------------------ |
| **`0x01`**     | Slave -> Host | Slave Control Command           | Low            | Could-Have   | Primarily Backlight Brightness              |
| **`0x02`**     | Slave -> Host | Button Command                  | **High**       | **Must-Have**  | Steering Wheel & Panel Buttons             |
| **`0x21`**     | Slave -> Host | Air Conditioning Info           | Medium         | Should-Have  | For 14款408 format - Check 407 applicability |
| **`0x29`**     | Slave -> Host | Steering Wheel Angle            | Medium         | Should-Have  | If needed for display/reverse lines         |
| **`0x30`**     | Slave -> Host | Radar Info (All-Around)         | **High**       | Should-Have  | Parking Sensors (If equipped)              |
| **`0x32`**     | Slave -> Host | Reverse Radar Info              | **High**       | Should-Have  | Parking Sensors (Rear specific?)            |
| **`0x33`**     | Slave -> Host | Trip Computer Info Page 0       | Medium         | Should-Have  | Instantaneous Consumption, Range          |
| **`0x34`**     | Slave -> Host | Trip Computer Info Page 1       | Medium         | Should-Have  | Trip 1 Averages                           |
| **`0x35`**     | Slave -> Host | Trip Computer Info Page 2       | Medium         | Should-Have  | Trip 2 Averages                           |
| **`0x36`**     | Slave -> Host | Outside Temperature             | **High**       | Should-Have  | Ambient Temperature                         |
| **`0x37`**     | Slave -> Host | Vehicle Alert Records           | Low            | Could-Have   | List of stored fault codes/warnings       |
| **`0x38`**     | Slave -> Host | Vehicle Status                  | **Very High**  | **Must-Have**  | Doors, Lights, Settings Status            |
| **`0x39`**     | Slave -> Host | Vehicle Function Status         | Low            | Could-Have   | Specific models (301, C4L, etc.)          |
| **`0x3A`**     | Slave -> Host | Diagnostic Information          | Low            | Could-Have   | List of active diagnostic issues          |
| **`0x3D`**     | Slave -> Host | Trip Computer Clear Status      | Low            | -            | Acknowledges trip clear command? (C4 specific?) |
| **`0x3B`**     | Slave -> Host | Memorized Speed Values          | Low            | Could-Have   | For Speed Limiter/Cruise (Specific models) |
| **`0x3D`**     | Slave -> Host | Cruise/Limit Speed Settings     | Low            | Could-Have   | For Speed Limiter/Cruise (Specific models) |
| **`0x3F`**     | Slave -> Host | Cruise/Limit Popup Request      | Low            | Could-Have   | For Speed Limiter/Cruise (DS6)            |
| **`0x7F`**     | Slave -> Host | Version Information             | Low            | Could-Have   | CANbox Firmware Version                   |
| ---            | ---           | ---                             | ---            | ---          | ---                                         |
| **`0x80`**     | Host -> Slave | Vehicle Parameter Setting       | Low            | Could-Have   | Command to change car settings            |
| **`0x82`**     | Host -> Slave | Trip Computer Parameter Setting | Low            | Could-Have   | Command to set trip distance/clear        |
| **`0x85`**     | Host -> Slave | Request Alert Records           | Low            | -            | Command to request `0x37` data            |
| **`0x86`**     | Host -> Slave | Request Function Status         | Low            | -            | Command to request `0x39` data            |
| **`0x87`**     | Host -> Slave | Request Diagnostic Info         | Low            | -            | Command to request `0x3A` data            |
| **`0x88`**     | Host -> Slave | Set Memorized Speed Values      | Low            | -            | Command for Speed Limiter/Cruise          |
| **`0x89`**     | Host -> Slave | Set Cruise/Limit Speed Settings | Low            | -            | Command for Speed Limiter/Cruise          |
| **`0x8A`**     | Host -> Slave | Air Conditioning Setting        | Low            | Could-Have   | Command to control AC (14款408 specific?) |
| **`0x8F`**     | Host -> Slave | Request Display Information     | Low            | -            | Generic request for status messages       |
| **`0x99`**     | Host -> Slave | Cruise/Limit Instrument Setting | Low            | -            | Command for Speed Limiter/Cruise          |
| **`0xA6`**     | Host -> Slave | Time Setting Information        | Low            | Could-Have   | Command to set vehicle time               |

**4.1 Detailed Payload Descriptions (Slave -> Host)**

*(Focusing on higher priority messages)*

**DataType `0x02`: Button Command**
*   `Length`: 0x04
*   `Data0`: Key Code (See table below)
*   `Data1`: Button Status (`0x01`=Pressed, `0x00`=Released - *Inferred*, verify!)
*   `Data2`: Reserved (Likely `0x00`)

    **Key Codes (Data0):**
    *   `0x02`: Menu
    *   `0x03`: Up (Menu Arrow)
    *   `0x04`: Down (Menu Arrow)
    *   `0x07`: OK
    *   `0x08`: Esc
    *   `0x10`: Mode (Display Mode Toggle)
    *   `0x11`: Source/Phone (SRC normally, PHONE during call)
    *   `0x12`: Seek+ / Next Track
    *   `0x13`: Seek- / Previous Track
    *   `0x14`: Vol+
    *   `0x15`: Vol-
    *   `0x16`: Mute
    *   `0x17`: Memo Up (Preset/List Up - Scroll Wheel Up?)
    *   `0x18`: Memo Down (Preset/List Down - Scroll Wheel Down?)
    *   `0x20`: Page SW (Trip Computer Page Toggle - Stalk Button)
    *   `0x21`: Menu Mode (?)
    *   `0x22`: Mem Button (Memory Speed Recall?)
    *   `0x23`: Bluetooth Button
    *   `0x29`: Push To Talk (Voice Command)
    *   `0x30`: Tel On (Answer)
    *   `0x31`: Tel Off (Hang Up)
    *   `0x32`-`0x37`: Navi, Radio, Setup, Addr, Media, Traf (Panel Buttons?)
    *   `0x38`-`0x43`: Up, Down, Left, Right, ScrollUp, ScrollDown (Center Console Controls?)
    *   `0x44`-`0x49`: 1-6 (Panel Preset Buttons)
    *   `0x4A`: SRC (Panel Button)
    *   `0x50`: BAND (Panel Button)
    *   `0x51`: LIST (Panel Button)
    *   `0x52`: Sound (Panel Button)
    *   `0x53`: TA/Info (Panel Button)
    *   `0x54`: Dark (Panel Button)
    *   `0x55`: Eject (Panel Button)
    *   `0x56`: >> (Panel Button)
    *   `0x57`: << (Panel Button)
    *   `0x58`: ▲ (Panel Button)
    *   `0x59`: ▼ (Panel Button)
    *   `0x60`: Check Button (Trigger Diagnostic Request?)
    *   `0x80`: Power Button (Panel)
    *   `0x00`: Release / No Press

**DataType `0x29`: Steering Wheel Angle** (14款408 specific in doc, but check if 407 uses it)
*   `Length`: 0x05
*   `Data0`: Angle Low Byte
*   `Data1`: Angle High Byte
    *   Value is Signed 16-bit integer (`(Data1 << 8) | Data0`).
    *   Range: -5450 to +5450 (Degrees * 10? Unit/Scaling needs verification). `0`=Center, `<0`=Right, `>0`=Left.

**DataType `0x30` / `0x32`: Radar Info**
*   `0x32` (Reverse Radar Info - Likely Rear Only):
    *   `Length`: 0x0A
    *   `Data0`: Radar Status (`00`/`01`=Enabled, `02`=Enabled+Display, `03`=Disabled)
    *   `Data1`: Rear Left Distance (0-5, 0=Closest, 5=Farthest/Inactive)
    *   `Data2`: Rear Middle Distance (0-5)
    *   `Data3`: Rear Right Distance (0-5)
    *   `Data4-6`: Front Sensor Distances (Likely `0x05` if only rear is active)
*   `0x30` (All-Around Radar - If front sensors present):
    *   `Length`: 0x09
    *   `Data0-2`: Front Left, Middle, Right Distance (0-5)
    *   `Data3-5`: Rear Left, Middle, Right Distance (0-5)
*   **Note:** Need to map the 0-5 range to what the Android HU expects (e.g., percentage, levels 0-7).

**DataType `0x33`: Trip Computer Page 0 (Instantaneous)**
*   `Length`: 0x0C
*   `Data0-1`: Instantaneous Fuel Consumption (L/100km * 10?, Big Endian `(D0<<8)|D1`, `0xFFFF`=Invalid) - **Verify unit/scaling**
*   `Data2-3`: Range (DTE) (km, Big Endian `(D2<<8)|D3`, `0xFFFF`=Invalid)
*   `Data4-5`: Set Destination Distance (km, Big Endian `(D4<<8)|D5`, `0xFFFF`=Invalid) - *Often unused by aftermarket.*
*   `Data6-8`: Start-Stop System Active Time (H:M:S, `FF`=Invalid) - *Likely not present on 2008 407.*

**DataType `0x34`: Trip Computer Page 1**
*   `Length`: 0x09
*   `Data0-1`: Average Fuel Consumption 1 (L/100km * 10?, BE `(D0<<8)|D1`, `FFFF`=Inv) - **Verify unit/scaling**
*   `Data2-3`: Average Speed 1 (km/h, BE `(D2<<8)|D3`, `FFFF`=Inv)
*   `Data4-5`: Distance Traveled 1 (km, BE `(D4<<8)|D5`, `FFFF`=Inv)

**DataType `0x35`: Trip Computer Page 2**
*   `Length`: 0x09
*   (Same structure as `0x34` but for Trip 2 data)

**DataType `0x36`: Outside Temperature**
*   `Length`: 0x04
*   `Data0`: Bit 7 = Sign (1=Negative), Bits 6-0 = Temperature Value (°C).

**DataType `0x38`: Vehicle Status**
*   `Length`: 0x09
*   **Data0 (Doors):** Bit 7=FL, 6=FR, 5=RL, 4=RR, 3=Trunk (1=Open)
*   **Data1 (Settings/Status 1):** Bit 7=Rear Wiper (1=En), Bit 6=Auto Park Brake (1=On, DS6?), Bit 4=Auto Door Lock (1=En, C4?), Bit 3=Park Assist Sys (1=En, 301/C4?), Bit 2=Central Lock (1=Locked, C4?), Bits 1-0=Current Trip Page (00=P0, 01=P1, 10=P2).
*   **Data2 (Settings/Status 2):** Bit 7=Daytime Lights (1=On), Bits 6-5=Headlight Delay (00=Off, 01=15s, 10=30s, 11=60s, C4L?), Bit 0=Adaptive Headlights (1=On).
*   **Data3 (Settings/Status 3):** Bit 7-5=Ambiance Light Level (0=Off, 1-7=Level), Bit 3=Park Sensor System Disable (1=Disabled), Bit 2=Reverse Status (1=Reverse), Bit 1=P Gear/Handbrake (1=P/On?), Bit 0=Parking Light Status (1=On).
*   **Data4 (Settings/Status 4):** Bits 7-6=FollowMeHome Light Time, Bits 5-4=Welcome Light Time, Bits 2-1=Audio Ambiance/EQ Setting, Bit 0=Fuel Unit Setting (0=KM/L, 1=L/100KM, 2=MPG).
*   **Data5 (Settings/Status 5):** Bit 7=Blind Spot Assist (1=On), Bit 6=Start/Stop System Disable (1=Disabled), Bit 5=Welcome Function (1=On), Bit 4=Driver-Only Door Unlock (0=Driver, 1=All), Bits 3-0=Language Setting.
*   **(Note:** Many bits in Data1-5 are model-specific, verify for 407).

### 5. Implementation Notes for `canbox-renode`

*   **Protocol Choice:** This `0xFD`-based RZC protocol is **different** from the `0x5A 0xA5`-based Hiworld PSA protocol (`input.md`). You need to implement **one or the other** based on what your specific CAN box hardware or Android head unit expects. Since your HU documentation might mention "Hiworld", the `0x5A` protocol might be the primary target, but this RZC documentation provides valuable PSA-specific data mapping insights.
*   **Mapping `carstate`:** Data decoded from CAN messages (e.g., speed from `0x28C`, doors from `0x131`, temp from `0x168`/`0x0F6`, SWC from `0x21F`) needs to be mapped and scaled correctly to fit the data fields defined in the chosen protocol (`0xFD` RZC or `0x5A` Hiworld).
*   **Button Handling:** Implement logic to detect button presses/releases from the chosen CAN source (`0x21F` or potentially `0x3E5` if keeping factory panel) and send the corresponding `0x02` message (for RZC) or `0x21` message (for Hiworld).
*   **Trip Data:** Implement handlers for the CAN IDs carrying trip data on your 407 (`0x221`, and the sniffed IDs for Trip 1/2, likely `0x2A1`/`0x261`) and format the data for the corresponding RZC (`0x33`/`0x34`/`0x35`) or Hiworld (`0x13`/`0x14`/`0x15`) messages.