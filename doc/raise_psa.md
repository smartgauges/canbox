# RZC PSA CANbox Protocol Documentation (for "Screen Removed" Installations)

**Document Version:** 1.6 (Verified Trip Computer scaling, Temperature mapping, noted Header/Length variations)

### 1. Introduction

This document describes the serial communication protocol used by RZC (睿志诚科技) CANbus decoders (CANbox) designed for Peugeot/Citroën (PSA) vehicles in scenarios where the **original factory display screen is removed**. It outlines the communication rules between an aftermarket Android head unit (DVD 主机) and the RZC CANbox (协议盒). Original source PDF: [RZC PSA Protocol PDF](https://github.com/cxsichen/helllo-world/blob/master/%E5%8D%8F%E8%AE%AE/%E7%9D%BF%E5%BF%97%E8%AF%9A/%E6%A0%87%E8%87%B4/%E6%A0%87%E8%87%B4%E9%9B%AA%E9%93%81%E9%BE%99%E6%8B%86%E5%8E%9F%E8%BD%A6%E5%B1%8F%E9%80%9A%E4%BF%A1%E5%8D%8F%E8%AE%AEV2.5.2.pdf)

This protocol allows the aftermarket head unit to receive vehicle status information (previously shown on the factory screen) and potentially send control commands.

**Target Functionality (as implied by the protocol & observed logs):**

*   Receive basic signals (ACC, Illumination, Reverse, Park - inferred from CANbox logic based on CAN).
*   Receive steering wheel and panel button presses (DataType `0x02`).
*   Receive steering wheel angle (DataType `0x29`).
*   Receive parking sensor data (DataType `0x32`).
*   **Receive trip computer data (Instantaneous, Trip 1, Trip 2 - DataTypes `0x33`, `0x34`, `0x35`) with consumption in L/100km.** (**REQUIRED**)
*   Receive outside temperature (DataType `0x36`).
*   Receive various vehicle status indicators (doors, lights, warnings - DataType `0x38`).
*   Receive diagnostic/alert information (DataTypes `0x37`, `0x3A`).
*   *Potentially* receive/send other data like AC info (`0x21`), settings, version info (`0x7F`), etc. (Less frequently observed or requires specific vehicle/CANbox features).

### 2. Physical Layer

Communication uses a standard **UART** interface.

*   **Baud Rate:** **19200 bps** (**Confirmed** by target configuration and log context).
*   **Data Bits:** 8
*   **Parity:** None
*   **Stop Bits:** 1
*   **Flow Control:** None
*   **Logic Level:** Assumed 3.3V (Verify with specific CANbox hardware).

### 3. Link Layer

**3.1 Frame Structure:**

Data is exchanged using frames with the following structure:

`HEADER (1 byte) | LENGTH (1 byte) | DATATYPE (1 byte) | DATA (n bytes) | CHECKSUM (1 byte)`

*   **HEADER:** Fixed value `0xFD` (**According to Specification**).
    *   **Log Analysis Note:** Some logs from specific implementations show `0xDF` being used instead of `0xFD`. **Verify which header byte your target Head Unit expects.** This documentation will assume `0xFD` per the original spec unless noted otherwise for specific implementations.
*   **LENGTH (`LEN`):** 1 byte. Specifies `1 + n` (1 for DataType byte + n for number of Data bytes).
    *   **Log Analysis Note:** For certain DataTypes (e.g., `0x02`, `0x29`, `0x32`), some logged frames show a `LEN` value that implies more data bytes than are visibly present before the checksum (e.g., `LEN=0x04` sent for `0x36` which only has 1 data byte). However, the checksum calculation consistently works based on `LEN + DType + Actual_Data_Bytes_Present`.
    *   **Implementation Guidance:**
        *   **Sending:** Calculate `LEN = 1 + actual_data_payload_size`. Send the *correct* `LEN` based on the data being sent (e.g., `LEN=0x02` for `0x36`, `LEN=4` for `0x02`, `LEN=3` for `0x29`, `LEN=8` for `0x32`, `LEN=12` for `0x33`, `LEN=7` for `0x34`/`0x35`).
        *   **Parsing:** Use the received `LEN` byte to determine expected frame size (`3 + LEN`). Read `LEN - 1` data bytes after the `DataType`. Validate checksum using the received `LEN` and all read data bytes. Be tolerant of implementations that might send an unexpected `LEN` but a valid checksum based on the data *actually* sent.
*   **DATATYPE (`Zone`):** 1 byte. Command Identifier. Defines the purpose/content of the message.
*   **DATA:** `n` bytes (`Data0` to `Datan`). Payload, `n = LEN - 1`.
*   **CHECKSUM (`CS`):** 1 byte. Calculated as the **sum of `Length`, `DataType`, `Data0`, ... `Datan`**. The sum is performed using 8-bit arithmetic (modulo 256, overflows are ignored). (**Calculation method confirmed**).
    *   **Log Analysis Note:** Some specific `0x02` (Button) frames in *one* analyzed log showed checksum mismatches. A second log showed correct checksums for `0x02`. Assume the standard calculation is correct unless widespread errors are seen during testing.

**3.2 Checksum Calculation Example:**

*(Using confirmed 0x36 data)* For frame `DF 04 36 14 4E` (20°C, Header DF, LEN 04, DType 36, Data0 14, CS 4E):
1. Bytes included in sum (based on matching CS and *actual data present*): `LEN=0x04`, `DType=0x36`, `Data0=0x14`
2. Sum = `0x04 + 0x36 + 0x14 = 0x4E`
3. Checksum = `0x4E` (Matches the log)

**3.3 Acknowledgement:**

The documentation does not specify an ACK/NACK mechanism. Assume messages are sent without expecting a link-layer confirmation.

### 4. Application Layer (DataType Definitions)

**Notation:**

*   **Slave:** CAN BUS Decoder (CANbox)
*   **Host:** DVD Navigation Host (Android Head Unit)
*   **Relevance:** Estimated relevance for the `canbox-renode` proxy project.
*   **Log Confirmed:** Indicates if this DataType was observed and generally aligns with the spec based on log analysis.

*(Table updated with Log Confirmed status and notes)*

| DataType (Hex) | Direction     | Description                     | Log Confirmed | Relevance      | Priority     | Notes                                       |
| :------------- | :------------ | :------------------------------ | :------------ | :------------- | :----------- | :------------------------------------------ |
| **`0x01`**     | Slave -> Host | Slave Control Command           | No            | Low            | Could-Have   | Primarily Backlight Brightness              |
| **`0x02`**     | Slave -> Host | Button Command                  | **Yes**       | **High**       | **Must-Have**  | Steering Wheel & Panel Buttons. **LEN/CS variance noted.** |
| **`0x21`**     | Slave -> Host | Air Conditioning Info           | No            | Medium         | Should-Have  | Spec says 14款408 format - Verify for target car. |
| **`0x29`**     | Slave -> Host | Steering Wheel Angle            | **Yes**       | Medium         | Should-Have  | Format confirmed, **LEN byte ambiguity noted.** |
| **`0x30`**     | Slave -> Host | Radar Info (All-Around)         | No            | Medium         | Should-Have  | Use `0x32` if only rear/basic needed.       |
| **`0x32`**     | Slave -> Host | Reverse Radar Info              | **Yes**       | **High**       | Should-Have  | Format/Checksum confirmed. Appears primary for reverse. **LEN byte ambiguity noted.** |
| **`0x33`**     | Slave -> Host | Trip Computer Info Page 0       | **Yes**       | **High**       | **Must-Have**  | Inst Cons (L/100km * 10), Range (km). **Scaling VERIFIED.** LEN=0x0C expected. |
| **`0x34`**     | Slave -> Host | Trip Computer Info Page 1       | **Yes**       | **High**       | **Must-Have**  | Trip 1 Averages (Cons L/100km * 10, Speed km/h, Dist km). **Scaling VERIFIED.** **LEN Discrepancy (Send LEN=7).** |
| **`0x35`**     | Slave -> Host | Trip Computer Info Page 2       | **Yes**       | **High**       | **Must-Have**  | Trip 2 Averages (Cons L/100km * 10, Speed km/h, Dist km). **Scaling VERIFIED.** **LEN Discrepancy (Send LEN=7).** |
| **`0x36`**     | Slave -> Host | Outside Temperature             | **Yes**       | **High**       | Should-Have  | **Payload mapping VERIFIED (Sign Bit + 7 Value Bits). LEN discrepancy noted.** |
| **`0x37`**     | Slave -> Host | Vehicle Alert Records           | **Yes**       | Low            | Could-Have   | Confirmed used for errors.                  |
| **`0x38`**     | Slave -> Host | Vehicle Status                  | **Yes**       | **Very High**  | **Must-Have**  | Doors, ParkLights, ParkBrake, Reverse confirmed. NearLights NOT included. ACC state implied. |
| **`0x39`**     | Slave -> Host | Vehicle Function Status         | No            | Low            | Could-Have   | Specific models (301, C4L, etc.)          |
| **`0x3A`**     | Slave -> Host | Diagnostic Information          | **Yes**       | Low            | Could-Have   | Confirmed used for errors/diagnostics.    |
| **`0x3D`**     | Slave -> Host | Trip Computer Clear Status      | No            | Low            | -            | Acknowledges trip clear command? (C4 specific?) |
| **`0x3B`**     | Slave -> Host | Memorized Speed Values          | No            | Low            | Could-Have   | For Speed Limiter/Cruise (Specific models) |
| **`0x3D`**     | Slave -> Host | Cruise/Limit Speed Settings     | No            | Low            | Could-Have   | For Speed Limiter/Cruise (Specific models) |
| **`0x3F`**     | Slave -> Host | Cruise/Limit Popup Request      | No            | Low            | Could-Have   | For Speed Limiter/Cruise (DS6)            |
| **`0x7F`**     | Slave -> Host | Version Information             | No            | Low            | Could-Have   | CANbox Firmware Version                   |
| ---            | ---           | ---                             | ---           | ---            | ---          | ---                                         |
| **`0x7D`**     | Host -> Slave | Vehicle Settings Command 2      | **Yes**       | Low            | Could-Have   | Seen in log, likely from HU trying settings. |
| *(Other Host->Slave omitted for brevity as not seen in log)* | | | | | | |
| ---            | ---           | ---                             | ---           | ---            | ---          | ---                                         |
| **`0x54`**     | Slave -> Host | *Unknown*                       | **Yes**       | Low?           | Won't Have   | **Not in spec doc.** Observed LEN=0x0B. Function unknown. Possibly misidentified `0x02` code `54`(DARK)? |
| **`0x55`**     | Slave -> Host | *Unknown*                       | **Yes**       | Low?           | Won't Have   | **Not in spec doc.** Observed LEN=0x0B. Function unknown. Possibly misidentified `0x02` code `55`(EJECT)? |
| **`0x56`**     | Slave -> Host | *Unknown*                       | **Yes**       | Low?           | Won't Have   | **Not in spec doc.** Observed LEN=0x0B. Function unknown. Possibly misidentified `0x02` code `56`(>>)? |

**4.1 Detailed Payload Descriptions (Slave -> Host)**

*(Focusing on higher priority messages, with Log Analysis notes and corrections)*

**DataType `0x02`: Button Command**
*   `Length`: `0x04` (Recommended Send Length: 1 DType + 3 Data = 4)
*   `Data0`: Key Code (See table below)
*   `Data1`: Button Status (`0x01`=Pressed, `0x00`=Released - **Confirmed**)
*   `Data2`: Reserved (`0x00`)
*   **Log Analysis Note:** Some logged frames had checksum errors or appeared truncated before CS. Recommend sending full 3 data bytes unless issues arise.

    **Key Codes (Data0):** *(Includes observed/confirmed codes)*
    *   `0x02`: Menu (**Confirmed**)
    *   `0x03`: Up (**Confirmed**)
    *   `0x04`: Down (**Confirmed**)
    *   `0x07`: OK (**Confirmed**)
    *   `0x08`: Esc (**Confirmed**)
    *   `0x09`: *Unknown* (Seen in log, maybe List/Center press?)
    *   `0x10`: Mode (**Confirmed**)
    *   `0x11`: Source/Phone (**Confirmed**)
    *   `0x12`: Seek+ / Next Track (**Confirmed**)
    *   `0x13`: Seek- / Previous Track (**Confirmed**)
    *   `0x14`: Vol+ (**Confirmed**)
    *   `0x15`: Vol- (**Confirmed**)
    *   `0x16`: Mute
    *   `0x17`: Memo Up (Scroll Wheel Up? - **Confirmed**)
    *   `0x18`: Memo Down (Scroll Wheel Down? - **Confirmed**)
    *   `0x20`: Page SW (Trip Computer Stalk Button - **Confirmed**)
    *   `0x23`: Bluetooth Button
    *   `0x29`: Push To Talk (Voice Command - **Confirmed**)
    *   `0x30`: Tel On (Answer - **Confirmed**)
    *   `0x31`: Tel Off (Hang Up)
    *   `0x38`-`0x3B`: Up, Down, Left, Right (Center Console Controls? - **Confirmed seen**)
    *   `0x44`-`0x49`: 1-6 (Panel Preset Buttons - **Confirmed seen**)
    *   `0x54`: Dark (Panel Button - **Confirmed**)
    *   `0x55`: Eject (Panel Button)
    *   `0x56`: >> (Panel Button)
    *   `0x60`: Check Button (**Confirmed**)
    *   `0x80`: Power Button (Panel)
    *   `0x00`: Release / No Press (**Confirmed**)
    *   *(Other documented codes likely valid)*

**DataType `0x29`: Steering Wheel Angle**
*   `Length`: `0x03` (Recommended Send Length: 1 DType + 2 Data = 3)
*   `Data0`: Angle Low Byte
*   `Data1`: Angle High Byte
    *   Value is Signed 16-bit integer (`(Data1 << 8) | Data0`).
    *   Range: -5450 to +5450 (Degrees * 10?). `0`=Center. **Spec: `<0`=Right, `>0`=Left.** (Verify scaling and direction against `carstate`).
    *   **Log Analysis Note:** Log line showed `LEN=0x05`, but payload `00 00` and matching CS `2E`. Recommend sending `LEN=0x03`.

**DataType `0x32`: Reverse Radar Info** (**Confirmed** primary radar type)
*   `Length`: `0x08` (Recommended Send Length: 1 DType + 7 Data = 8)
*   `Data0`: Radar Status (`02`=Enabled+Display, `03`=Disabled - **Confirmed**)
*   `Data1`: Rear Left Distance (0-5, 0=Closest, 5=Inactive - **Mapping Confirmed**)
*   `Data2`: Rear Middle Distance (0-5)
*   `Data3`: Rear Right Distance (0-5)
*   `Data4`: Front Left Distance (0-5)
*   `Data5`: Front Middle Distance (0-5)
*   `Data6`: Front Right Distance (0-5)
*   **Log Analysis Note:** Log line showed `LEN=0x0A`, but only 7 data bytes are documented/needed. Send with `LEN=0x08`.

**DataType `0x33`: Trip Computer Page 0 (Instantaneous)**
*   `Length`: `0x0C` (Expected: 1 DType + 11 Data = 12)
*   `Data0-1`: **Instantaneous Fuel Consumption**. Unit: **L/100km * 10**. Big Endian `(D0<<8)|D1`. `0xFFFF` displays as "--.-". (**Scaling Verified**)
*   `Data2-3`: **Range (DTE)**. Unit: **km**. Big Endian `(D2<<8)|D3`. `0xFFFF` displays as "----". (**Scaling Verified**)
*   `Data4-5`: Set Destination Distance (km, BE `(D4<<8)|D5`, `0xFFFF`=Invalid) - *Often unused.*
*   `Data6-8`: Start-Stop System Active Time (H:M:S, `FF`=Invalid) - *Likely N/A for target.*
*   `Data9-10`: Reserved/Unknown. Send as `0x00`?

**DataType `0x34`: Trip Computer Page 1**
*   `Length`: **`0x07`** (Recommended Send Length: 1 DType + 6 Data = 7). **Note:** Spec doc shows `LEN=0x09`. Send `LEN=0x07`.
*   `Data0-1`: **Average Fuel Consumption 1**. Unit: **L/100km * 10**. Big Endian `(D0<<8)|D1`. `0xFFFF` displays as "--.-". (**Scaling Verified**)
*   `Data2-3`: **Average Speed 1**. Unit: **km/h**. Big Endian `(D2<<8)|D3`. `0xFFFF` displays as "---". (**Scaling Verified**)
*   `Data4-5`: **Distance Traveled 1**. Unit: **km**. Big Endian `(D4<<8)|D5`. `0xFFFF` displays as "----". (**Scaling Verified**)

**DataType `0x35`: Trip Computer Page 2**
*   `Length`: **`0x07`** (Recommended Send Length: 1 DType + 6 Data = 7). **Note:** Spec doc shows `LEN=0x09`. Send `LEN=0x07`.
*   `Data0-1`: **Average Fuel Consumption 2**. Unit: **L/100km * 10**. Big Endian `(D0<<8)|D1`. `0xFFFF` displays as "--.-". (**Scaling Verified**)
*   `Data2-3`: **Average Speed 2**. Unit: **km/h**. Big Endian `(D2<<8)|D3`. `0xFFFF` displays as "---". (**Scaling Verified**)
*   `Data4-5`: **Distance Traveled 2**. Unit: **km**. Big Endian `(D4<<8)|D5`. `0xFFFF` displays as "----". (**Scaling Verified**)

**DataType `0x36`: Outside Temperature**
*   `Length`: `0x02` (Recommended Send Length: 1 DType + 1 Data = 2). **Note:** Some logs show `LEN=0x04` being sent. Recommend sending `LEN=0x02`.
*   `Data0`: Temperature Value (°C).
    *   **Format:** Bit 7 = Sign (0 = Positive, 1 = Negative). Bits 6-0 = Absolute temperature value. (**Format Verified**)
    *   **Implementation:**
        *   To encode: If `temp_C < 0`, `payload = 0x80 | (-temp_C & 0x7F)`. Else, `payload = temp_C & 0x7F`. Clamp `temp_C` to -127..+127 range before encoding.
        *   To decode: If `payload & 0x80`, `temp_C = -(payload & 0x7F)`. Else, `temp_C = payload & 0x7F`.

**DataType `0x38`: Vehicle Status**
*   `Length`: `0x07` (**Confirmed**)
*   **Data0 (Doors):** Bit 7=FL, 6=FR, 5=RL, 4=RR, 3=Trunk (1=Open - **Confirmed**)
*   **Data1 (Settings/Status 1):** Bit 3=Park Assist Sys Active (1=Enabled - **Confirmed**). Other bits vehicle/config dependent.
*   **Data2 (Settings/Status 2):** Vehicle/config dependent.
*   **Data3 (Settings/Status 3):** Bit 2=Reverse Status (1=Reverse - **Confirmed**), Bit 1=Park Brake (1=On - **Confirmed**), Bit 0=Parking Light (1=On - **Confirmed**). Other bits vehicle/config dependent.
*   **Data4/5 (Settings/Status 4/5):** Vehicle/config dependent.
*   **Log Analysis Note:** Near light status is NOT present. State seems to reset to mostly zeros when ACC/IGN is off.

[...] *(Other DataTypes can be added here if needed)*

### 5. Implementation Notes for `canbox-renode`

*   **Protocol Selection:** Ensure `-DE_CANBOX_RAISE_RCZ` build flag, correct `e_canbox_t` enum (`e_cb_raiserzc` - **add this enum if missing**), and **19200 baud** are set.
*   **Header Byte:** **Confirm target HU needs `FD` (spec) or `DF` (logs).** Modify `snd_raise_rcz_msg` in `utils.c` (or create a new sender function) accordingly. Default to `FD`.
*   **Temperature Mapping (`0x36`):** Implement the **Sign Bit + 7 Value Bits** encoding in `raiserzc.c`. Use the `car_get_temp()` value.
*   **Trip Computer Data (`0x33`, `0x34`, `0x35`):**
    *   **Confirm CAN Sources:** Identify/confirm CAN IDs for Trip 1 (`0x2A1`?) / Trip 2 (`0x261`?) data on the Peugeot 407. Use `0x221` for instantaneous. Implement handlers in `peugeot_407.c`.
    *   **Implement Scaling:** Use the **verified scaling factors**: Consumption = L/100km * 10, Speed = km/h, Distance = km. Apply appropriate scaling when reading from `carstate` and formatting the 16-bit Big Endian values in `raiserzc.c`.
    *   **Handle Invalid States:** Map invalid CAN readings to `0xFFFF` in the RZC payload.
*   **Length Handling:** **Send messages using the CORRECT `LEN` byte value calculated as `LEN = 1 + actual_data_payload_size`**, based on the *documented/verified data structures*. Specifically:
    *   `0x02`: LEN=4
    *   `0x29`: LEN=3
    *   `0x32`: LEN=8
    *   `0x33`: LEN=12 (0x0C)
    *   `0x34`/`0x35`: LEN=7
    *   `0x36`: LEN=2
    *   `0x38`: LEN=7
*   **Checksum:** Use the standard RZC checksum calculation (`Sum(LEN, DTYPE, DATA...) & 0xFF`).
*   **Button Mapping:** Verify the RZC key codes used in the `raise_rcz_*` callback functions match the target vehicle's buttons.
*   **Radar Mapping:** Implement the `map_radar_dist_to_rzc` helper correctly (0-7 -> 0-5).
*   **Ignore Unknown Types:** Do not implement sending/handling for `0x54`, `0x55`, `0x56` unless their function is confirmed and required.