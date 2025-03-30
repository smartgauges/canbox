# Hiworld PSA CANbox Protocol Documentation

## 1. Introduction

This document describes the serial communication protocol used by Hiworld Technology Co., Ltd. CANbus decoders (CANbox) specifically designed for Peugeot/Citroën (PSA) vehicles. It outlines the communication rules between an aftermarket Android head unit (DVD 主机) and the Hiworld PSA CANbox (协议盒), covering the physical layer, data link layer, and application layer.

This documentation is based on the "Hiworld 标致-雪铁龙 PSA CANbox 协议盒全兼容通讯协议 Ver 2.5 - Apr 2015" document. [https://github.com/cxsichen/helllo-world](https://github.com/cxsichen/helllo-world/blob/master/%E5%8D%8F%E8%AE%AE/%E5%B0%9A%E6%91%84/%E6%A0%87%E8%87%B4%E5%85%A8%E5%85%BC%E5%AE%B9%E5%8D%8F%E8%AE%AE%E8%B5%84%E6%96%99(1)/CANode_Communications%20Protocol_PSA_V2.6_2015.04.03.pdf)

The CANbox aims to provide the following functionalities to the head unit:

*   ACC signal status
*   Illumination signal status
*   Reverse gear signal status
*   Parking brake signal status
*   Original panel button/knob signals
*   Steering wheel control signals
*   Outside temperature
*   Parking sensor (radar) signals
*   Door status signals
*   Seat belt status signals
*   Trip computer information
*   Diagnostic functions support
*   Vehicle settings interaction

## 2. Physical Layer

Communication between the head unit and the CANbox uses a UART (Universal Asynchronous Receiver/Transmitter) interface with the following parameters:

*   **Baud Rate:** 38400 bps
*   **Data Bits:** 8
*   **Parity:** None
*   **Stop Bits:** 1
*   **Flow Control:** None
*   **Logic Level:** Typically 3.3V (Verify based on specific CANbox hardware)

*Note: RX/TX directions are relative to the CANbox.*

| CANbox Pin | Head Unit Pin | Description                     |
| :--------- | :------------ | :------------------------------ |
| TX         | RX            | CANbox transmits, Head Unit receives |
| RX         | TX            | CANbox receives, Head Unit transmits |
| GND        | GND           | Common Ground                   |

## 3. Link Layer

### 3.1 Frame Structure

Data is transmitted in frames with the following structure:

`HEADER (2 bytes) | LENGTH (1 byte) | ComID (1 byte) | DATA (n bytes) | CHECKSUM (1 byte)`

*   **HEADER:** Fixed value `0x5A 0xA5`. Indicates the start of a frame.
*   **LENGTH (`LEN`):** 1 byte. Specifies the number of bytes following the LENGTH byte, *including* `ComID` and `DATA`, but *excluding* `CHECKSUM`. So, `LEN = 1 (ComID) + n (DATA bytes)`.
*   **ComID:** 1 byte. Command Identifier, defines the purpose of the message.
*   **DATA:** `n` bytes. The actual payload of the message, where `n = LEN - 1`. The structure depends on the `ComID`.
*   **CHECKSUM (`CS`):** 1 byte. Calculated as `(LEN + ComID + SUM(DATA bytes)) - 1`. The sum is calculated arithmetically, ignoring overflows (treating bytes as `uint8_t`).

### 3.2 Checksum Calculation Example

For the frame `5A A5 07 11 88 10 01 00 00 00 CS`:

1.  `LEN = 0x07`
2.  `ComID = 0x11`
3.  `DATA = 88 10 01 00 00 00`
4.  `Sum = LEN + ComID + DATA[0] + ... + DATA[5]`
    `Sum = 0x07 + 0x11 + 0x88 + 0x10 + 0x01 + 0x00 + 0x00 + 0x00 = 0xB1`
5.  `Checksum = Sum - 1 = 0xB1 - 1 = 0xB0`
6.  Full frame: `5A A5 07 11 88 10 01 00 00 00 B0`

*Note: The C implementation uses `uint8_t sum = 0; ... sum += byte; ... sum = sum - 1;`. This naturally handles overflows.*

### 3.3 Acknowledgement Frames

*   **ACK (Acknowledge):** Sent by the receiver to confirm successful reception and valid checksum.
    *   Format: `5A A5 01 FF ComID CS`
    *   Example: `5A A5 01 FF 21 20` (ACK for ComID 0x21)
*   **NACK (Not Acknowledge):** Sent by the receiver if the checksum is invalid.
    *   Format: `5A A5 01 FE ComID CS`
    *   Example: `5A A5 01 FE 21 1F` (NACK for ComID 0x21)

## 4. Application Layer

This section describes the `ComID`s and their associated data payloads.

### 4.1 ComID Definitions

| No. | ComID | Sender        | Receiver    | Definition & Description                 | Relevant to `canbox`? | Priority (Proxy) |
| :-: | :---- | :------------ | :---------- | :--------------------------------------- | :------------------: | :---------------: |
| 1   | 0x11  | CANbox        | Head Unit   | Basic Vehicle Information              | Yes                  | Must-Have        |
| 2   | 0x12  | CANbox        | Head Unit   | Detailed Vehicle Information           | Yes                  | Should-Have      |
| 3   | 0x13  | CANbox        | Head Unit   | Trip Computer Info Page 0              | Yes                  | Could-Have       |
| 4   | 0x14  | CANbox        | Head Unit   | Trip Computer Info Page 1              | Yes                  | Could-Have       |
| 5   | 0x15  | CANbox        | Head Unit   | Trip Computer Info Page 2              | Yes                  | Could-Have       |
| 6   | 0x1B  | Head Unit     | CANbox      | Trip Information Settings Command        | Maybe (Low)          | Could-Have       |
| 7   | 0x21  | CANbox        | Head Unit   | Panel Key Presses                      | Yes                  | Must-Have        |
| 8   | 0x22  | CANbox        | Head Unit   | Panel Knob Rotation                    | Yes                  | Should-Have      |
| 9   | 0x24  | Head Unit     | CANbox      | Vehicle Model Setting                  | Yes (Config)         | Should-Have      |
| 10  | 0x31  | CANbox        | Head Unit   | Air Conditioning Information           | Yes                  | Should-Have      |
| 11  | 0x3B  | Head Unit     | CANbox      | Control Air Conditioning Command       | Maybe (AC Control)   | Could-Have       |
| 12  | 0x41  | CANbox        | Head Unit   | Radar (Parking Sensor) Information     | Yes                  | Should-Have      |
| 13  | 0x42  | CANbox        | Head Unit   | Alert Information                      | Maybe (Low)          | Could-Have       |
| 14  | 0x71  | CANbox        | Head Unit   | Vehicle Settings Enable 1              | Maybe (Low)          | Could-Have       |
| 15  | 0x72  | CANbox        | Head Unit   | Vehicle Settings Enable 2              | Maybe (Low)          | Could-Have       |
| 16  | 0x76  | CANbox        | Head Unit   | Vehicle Settings Information           | Maybe (Low)          | Could-Have       |
| 17  | 0x7B  | Head Unit     | CANbox      | Vehicle Settings Command               | Maybe (Low)          | Could-Have       |
| 18  | 0x79  | CANbox        | Head Unit   | Vehicle Settings Information 2         | Maybe (Low)          | Could-Have       |
| 19  | 0x7D  | Head Unit     | CANbox      | Vehicle Settings Command 2             | Maybe (Low)          | Could-Have       |
| 20  | 0xF0  | CANbox        | Head Unit   | Software Version Number                | Yes (Debug/Info)     | Could-Have       |
| 21  | 0x81  | CANbox        | Head Unit   | Speed Limit Memory Info                | Maybe (Low)          | Could-Have       |
| 22  | 0x8A  | Head Unit     | CANbox      | Speed Limit Memory Command             | Maybe (Low)          | Could-Have       |
| 23  | 0x82  | CANbox        | Head Unit   | Cruise Control Speed Info              | Maybe (Low)          | Could-Have       |
| 24  | 0x8B  | Head Unit     | CANbox      | Cruise Control Speed Command           | Maybe (Low)          | Could-Have       |
| 25  | 0x83  | CANbox        | Head Unit   | SOS Information                        | Maybe (Low)          | Could-Have       |
| 26  | 0x85  | CANbox        | Head Unit   | Sport Mode Settings Info               | Maybe (Low)          | Could-Have       |
| 27  | 0x8C  | Head Unit     | CANbox      | Sport Mode Settings Command            | Maybe (Low)          | Could-Have       |
| 28  | 0x94  | CANbox        | Head Unit   | Language Settings Info                 | Maybe (Low)          | Could-Have       |
| 29  | 0x9A  | Head Unit     | CANbox      | Language Settings Request Command      | Maybe (Low)          | Could-Have       |
| 30  | 0xC1  | CANbox        | Head Unit   | Unit Settings Info                     | Maybe (Low)          | Could-Have       |
| 31  | 0xCA  | Head Unit     | CANbox      | Unit Settings Command                  | Maybe (Low)          | Could-Have       |
| 32  | 0xC2  | CANbox        | Head Unit   | Time/Date Settings Info                | Maybe (Low)          | Could-Have       |
| 33  | 0xCB  | Head Unit     | CANbox      | Time/Date Settings Command             | Maybe (Low)          | Could-Have       |
| 34  | 0x21  | Head Unit     | CANbox      | *Simulated* Panel Key Press            | No (HU -> Car)       | -                |
| 35  | 0xA1  | Head Unit     | CANbox      | Host Display Status                    | No (HU -> Car)       | -                |
| 36  | 0xA2  | Head Unit     | CANbox      | Radio Information                      | No (HU -> Car)       | -                |
| 37  | 0xA3  | Head Unit     | CANbox      | Radio Preset Information               | No (HU -> Car)       | -                |
| 38  | 0xA4  | Head Unit     | CANbox      | CD/CDC Information                     | No (HU -> Car)       | -                |

---

### 4.2 Detailed ComID Descriptions

#### **ComID 0x11: Basic Vehicle Information**

*   **Direction:** CANbox -> Head Unit
*   **Length (`LEN`):** 0x07
*   **Description:** Provides essential status information about the vehicle.

| Byte   | Bit(s) | Name                  | Description                                                                                             | `car.c` Variable (Example) |
| :----- | :----- | :-------------------- | :------------------------------------------------------------------------------------------------------ | :------------------------- |
| Data 0 | 7      | Driver Door           | 1 = Open, 0 = Closed                                                                                    | `fl_door`                  |
|        | 6      | Passenger Door (FR)   | 1 = Open, 0 = Closed                                                                                    | `fr_door`                  |
|        | 5      | Rear Left Door        | 1 = Open, 0 = Closed                                                                                    | `rl_door`                  |
|        | 4      | Rear Right Door       | 1 = Open, 0 = Closed                                                                                    | `rr_door`                  |
|        | 3      | Trunk (Tailgate)      | 1 = Open, 0 = Closed                                                                                    | `tailgate`                 |
|        | 2      | Bonnet (Hood)         | 1 = Open, 0 = Closed                                                                                    | `bonnet`                   |
|        | 1-0    | Reserved              |                                                                                                         |                            |
| Data 1 | 7      | ACC Status            | 1 = ACC ON, 0 = ACC OFF                                                                                 | `acc`                      |
|        | 6      | Illumination Status   | 1 = Illumination Signal ON (Parking/Headlights ON), 0 = OFF                                             | `near_lights` / `illum`    |
|        | 5      | Reverse Status        | 1 = Reverse Gear Engaged, 0 = Not in Reverse                                                            | `get_rear_delay_state()`   |
|        | 4      | Parking Brake Status  | 1 = Parking Brake ON, 0 = OFF                                                                           | `park_break`               |
|        | 3      | Seat Belt Status      | 1 = Driver Seat Belt Unfastened, 0 = Fastened                                                           | `ds_belt`                  |
|        | 2      | Original Radar Status | 1 = Original parking radar system active/available, 0 = Inactive/Unavailable                           | `radar.state != e_radar_off`? |
|        | 1-0    | Reserved              |                                                                                                         |                            |
| Data 2 | 7-0    | Steering Wheel Angle  | Raw value from CAN bus? (Needs verification/scaling for specific vehicle)                                 | `wheel` (after scaling)    |
| Data 3 | 7-0    | Outdoor Temperature   | Value = Data - 40 (°C). Range: -40 to 87.5 °C. `0xFF` = Invalid.                                          | `temp`                     |
| Data 4 | 7-0    | Reserved              |                                                                                                         |                            |
| Data 5 | 7-0    | Reserved              |                                                                                                         |                            |

*Note: The exact mapping of Data 0/1 bits might vary slightly between PSA models. Verification with logs is recommended.*

---

#### **ComID 0x12: Detailed Vehicle Information**

*   **Direction:** CANbox -> Head Unit
*   **Length (`LEN`):** 0x07
*   **Description:** Provides VIN, Speed, and other detailed info.

| Byte   | Bit(s) | Name          | Description                                                                        | `car.c` Variable (Example) |
| :----- | :----- | :------------ | :--------------------------------------------------------------------------------- | :------------------------- |
| Data 0 | 7-0    | VIN ASCII 1   | First ASCII character of the last 8 digits of the VIN                              | `vin`[9]                   |
| Data 1 | 7-0    | VIN ASCII 2   | Second ASCII character...                                                          | `vin`[10]                  |
| Data 2 | 7-0    | VIN ASCII 3   | ...                                                                                | `vin`[11]                  |
| Data 3 | 7-0    | VIN ASCII 4   | ...                                                                                | `vin`[12]                  |
| Data 4 | 7-0    | VIN ASCII 5   | ...                                                                                | `vin`[13]                  |
| Data 5 | 7-0    | VIN ASCII 6   | Sixth ASCII character of the last 8 digits of the VIN                              | `vin`[14]                  |
| Data 6 | 7-0    | Vehicle Speed | Low Byte (Speed in km/h * 100). Full speed = (Data6 << 8 \| Data5). `0xFFFF` = Invalid | `speed`                    |
| Data 7 | 7-0    | Vehicle Speed | High Byte                                                                          | `speed`                    |

*Note: The source document seems incomplete for ComID 0x12. It only shows 8 data bytes but mentions VIN, speed, etc. The table above is an interpretation based on common practices and needs verification.*

---

#### **ComID 0x21: Panel Key Presses**

*   **Direction:** CANbox -> Head Unit
*   **Length (`LEN`):** 0x03
*   **Description:** Reports steering wheel button presses and releases.

| Byte   | Bit(s) | Name        | Description                                                              | `car.c` `key_cb_t` Function |
| :----- | :----- | :---------- | :----------------------------------------------------------------------- | :-------------------------- |
| Data 0 | 7-0    | Key Code    | See Key Code table below.                                                |                             |
| Data 1 | 7-0    | Key Status  | `0x01` = Pressed, `0x00` = Released                                      |                             |
| Data 2 | 7-0    | Reserved    |                                                                          |                             |

**Key Code Table (Data 0):**

| Key Code (Hex) | Button Description             | `key_cb_t` Function |
| :------------- | :----------------------------- | :------------------ |
| 0x01           | Volume +                       | `inc_volume`        |
| 0x02           | Volume -                       | `dec_volume`        |
| 0x03           | Mute                           | `mode`?             |
| 0x04           | Prev Track / Seek Down / Rew   | `prev`              |
| 0x05           | Next Track / Seek Up / FF      | `next`              |
| 0x06           | SRC (Source)                   | `mode`              |
| 0x07           | TEL (Phone Answer/Hang Up)     | `cont`?             |
| 0x08           | VR (Voice Recognition)         | `mici`?             |
| 0x09           | List / OK (Rotary Encoder Push)| ?                   |
| 0x0A           | Return / ESC                   | ?                   |
| 0x0B           | Menu                           | `mode`?             |
| 0x0C           | EQ / Sound Settings            | ?                   |
| 0x0D           | TA (Traffic Announcement)      | ?                   |
| 0x0E           | Dimmer / Dark                  | ?                   |
| 0x0F           | INFO / Trip                    | ?                   |
| 0x10           | Preset 1 / Phone 1             | ?                   |
| ...            | (See source PDF for others)    |                     |
| 0x1F           | Preset 6 / Phone #             | ?                   |
| 0x20           | Setup / Settings               | ?                   |
| 0x21           | Auto Scan / A/S                | ?                   |

*Note: The mapping of Key Codes to `key_cb_t` functions needs confirmation based on expected behavior.*

---

#### **ComID 0x22: Panel Knob Rotation**

*   **Direction:** CANbox -> Head Unit
*   **Length (`LEN`):** 0x02
*   **Description:** Reports rotation of knobs (e.g., volume knob).

| Byte   | Bit(s) | Name          | Description                                     | `car.c` `key_cb_t` Function |
| :----- | :----- | :------------ | :---------------------------------------------- | :-------------------------- |
| Data 0 | 7-0    | Knob ID       | `0x01` = Right Knob (usually Volume)            |                             |
| Data 1 | 7-0    | Rotation Step | Positive value = Clockwise (e.g., Volume Up)    | `inc_volume`                |
|        |        |               | Negative value = Counter-Clockwise (Volume Down)| `dec_volume`                |

*Note: The step value might represent the number of detents turned. A single step might be +1 or -1 (represented as 0xFF for -1 in `int8_t`).*

---

#### **ComID 0x24: Vehicle Model Setting**

*   **Direction:** Head Unit -> CANbox
*   **Length (`LEN`):** 0x02
*   **Description:** Sent by the head unit to inform the CANbox of the selected vehicle type/configuration.

| Byte   | Bit(s) | Name        | Description                              |
| :----- | :----- | :---------- | :--------------------------------------- |
| Data 0 | 7-0    | Vehicle ID  | See Appendix Table in source PDF.      |
| Data 1 | 7-0    | Option ID   | See Appendix Table in source PDF.      |

*Note: The CANbox firmware might use this information to adjust its behavior or select the correct CAN message decoding internally, although the current `canbox` implementation relies on compile-time or debug-mode selection.*

---

#### **ComID 0x31: Air Conditioning Information**

*   **Direction:** CANbox -> Head Unit
*   **Length (`LEN`):** 0x07
*   **Description:** Reports the status of the climate control system.

| Byte   | Bit(s) | Name                | Description                                                                                                | `car.c` Variable (Example)  |
| :----- | :----- | :------------------ | :--------------------------------------------------------------------------------------------------------- | :-------------------------- |
| Data 0 | 7      | AC System ON/OFF    | 1 = ON, 0 = OFF                                                                                            | `ac`? (Need confirmation) |
|        | 6      | AC MAX Mode         | 1 = ON, 0 = OFF                                                                                            | `ac_max`                  |
|        | 5      | Recirculation       | 1 = ON, 0 = OFF (Auto/Manual depends on Data 1 Bit 7)                                                      | `recycling`                 |
|        | 4      | Dual Zone Mode      | 1 = DUAL ON, 0 = DUAL OFF                                                                                  | `dual`                      |
|        | 3      | AUTO Mode           | 1 = ON, 0 = OFF                                                                                            |                             |
|        | 2      | Rear Defrost        | 1 = ON, 0 = OFF                                                                                            | `rear`                      |
|        | 1      | Reserved            |                                                                                                            |                             |
|        | 0      | AC Compressor Status| 1 = Compressor ON, 0 = OFF                                                                               | `ac`                      |
| Data 1 | 7      | Auto Recirculation  | 1 = Auto Recirculation ON, 0 = Manual Recirculation                                                      | `aqs`?                      |
|        | 6      | Reserved            |                                                                                                            |                             |
|        | 5-3    | Fan Speed           | 0x0 = OFF, 0x1-0x7 = Speed 1-7                                                                             | `fanspeed`                  |
|        | 2      | Airflow: Floor      | 1 = ON, 0 = OFF                                                                                            | `floor`                     |
|        | 1      | Airflow: Face/Mid   | 1 = ON, 0 = OFF                                                                                            | `middle`                    |
|        | 0      | Airflow: Windshield | 1 = ON, 0 = OFF                                                                                            | `wind`                      |
| Data 2 | 7-0    | Left Temp Setting   | See Temperature Table below.                                                                               | `l_temp`                    |
| Data 3 | 7-0    | Right Temp Setting  | See Temperature Table below.                                                                               | `r_temp`                    |
| Data 4 | 7-0    | Reserved            |                                                                                                            |                             |
| Data 5 | 7-0    | Reserved            |                                                                                                            |                             |
| Data 6 | 7-0    | Reserved            |                                                                                                            |                             |

**Temperature Table (Data 2, Data 3):**

| Value (Hex) | Temperature (°C) | Display |
| :---------- | :--------------- | :------ |
| 0x00        | ---              | LO      |
| 0x01        | 14.0             | 14.0    |
| 0x02        | 14.5             | 14.5    |
| ...         | ...              | ...     |
| 0x20        | 30.0             | 30.0    |
| 0xFE        | ---              | HI      |
| 0xFF        | Invalid/Sync     | ---     |

---

#### **ComID 0x41: Radar (Parking Sensor) Information**

*   **Direction:** CANbox -> Head Unit
*   **Length (`LEN`):** 0x0D (13 bytes)
*   **Description:** Provides distance information from front and rear parking sensors.

| Byte    | Bit(s) | Name                 | Description                                                                                   | `car.c` Variable (Example) |
| :------ | :----- | :------------------- | :-------------------------------------------------------------------------------------------- | :------------------------- |
| Data 0  | 7-0    | Rear Left            | Distance: 0x01 (Closest) to 0xFF (Farthest/No Object/Inactive)                               | `rl` (needs scaling/map) |
| Data 1  | 7-0    | Rear Left Middle     | Distance: 0x01 to 0xFF                                                                        | `rlm`                      |
| Data 2  | 7-0    | Rear Right Middle    | Distance: 0x01 to 0xFF                                                                        | `rrm`                      |
| Data 3  | 7-0    | Rear Right           | Distance: 0x01 to 0xFF                                                                        | `rr`                       |
| Data 4  | 7-0    | Front Right          | Distance: 0x01 to 0xFF                                                                        | `fr`                       |
| Data 5  | 7-0    | Front Right Middle   | Distance: 0x01 to 0xFF                                                                        | `frm`                      |
| Data 6  | 7-0    | Front Left Middle    | Distance: 0x01 to 0xFF                                                                        | `flm`                      |
| Data 7  | 7-0    | Front Left           | Distance: 0x01 to 0xFF                                                                        | `fl`                       |
| Data 8  | 7-0    | Reserved             |                                                                                               |                            |
| Data 9  | 7-0    | Reserved             |                                                                                               |                            |
| Data 10 | 7-0    | Reserved             |                                                                                               |                            |
| Data 11 | 7-0    | Parking System Status| `0x00` = OFF, `0x10` = Rear ON, `0x20` = Front ON, `0x30` = Front & Rear ON                   | `radar.state`              |

*Note: The distance values (0x01-0xFF) likely need to be mapped or scaled to a percentage or level (0-7, 0-10) for display, similar to other protocols. The exact mapping needs testing.*

---

#### **ComID 0xF0: Software Version Number**

*   **Direction:** CANbox -> Head Unit
*   **Length (`LEN`):** 0x04
*   **Description:** Reports the firmware version of the CANbox.

| Byte   | Bit(s) | Name           | Description                       |
| :----- | :----- | :------------- | :-------------------------------- |
| Data 0 | 7-0    | Version Main   | e.g., `0x01` for V1.xx            |
| Data 1 | 7-0    | Version Sub    | e.g., `0x20` for Vxx.20           |
| Data 2 | 7-0    | Version Detail | e.g., `0x15` for Vxx.xx Build 15 |
| Data 3 | 7-0    | Reserved       |                                   |

---

**(Other ComIDs are omitted for brevity but can be added following the same format based on the source PDF if needed for specific features.)**

## 5. Implementation Notes

*   **Checksum:** Ensure the checksum calculation in `utils.c` (`canbox_hiworld_checksum`) correctly matches the `(SUM) - 1` logic.
*   **Sending Messages:** Use the `snd_canbox_hiworld_msg` function from `utils.c` to format and send messages with the correct header and checksum.
*   **Receiving Commands:** Implement a state machine (similar to the one in `canbox.c` for the Raise protocol) to parse incoming `5A A5 ...` frames from the head unit, validate the checksum, and process the `ComID` and `DATA` for commands like `0x24` (Vehicle Type) or `0x3B` (AC Control).
*   **Data Mapping:** The data values received from the vehicle CAN bus (`carstate` struct) need to be correctly mapped and scaled to the formats expected by the Hiworld protocol (e.g., temperature offset, radar distance scaling, bit flags).
*   **Steering Wheel Keys:** The mapping between the CAN bus key codes (from `peugeot_407.c`) and the Hiworld protocol key codes (ComID `0x21`) needs to be implemented. The `key_cb_t` callbacks should trigger sending the appropriate `0x21` message.

This documentation provides a solid foundation for implementing the Hiworld PSA protocol in the `canbox` firmware for your Peugeot 407 project. Remember to verify message contents and data formats against your specific vehicle's CAN logs and the head unit's behavior.