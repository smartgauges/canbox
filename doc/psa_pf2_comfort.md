# PSA PF2 Platform CAN Message Reference (Derived from PSACANBridge)

**Source Repository:** [https://github.com/morcibacsi/PSACANBridge](https://github.com/morcibacsi/PSACANBridge)

### 1. Introduction

This document provides a reference for CAN bus messages relevant to PSA PF2 platform vehicles (e.g., Peugeot 407, Citroën C5 Mk1/Mk2, Peugeot 307/308 Mk1) based *specifically* on the reverse engineering and C code implementation found in the `morcibacsi/PSACANBridge` project by morcibacsi.

The primary focus is on messages typically found on the **CAN Comfort (CAN-INFO) bus**, which usually operates at **125 kbps**. This information is crucial for developing CAN interface devices like CAN boxes or custom integrations.

**Note:** While this data is derived from a working implementation, **always verify CAN IDs, data formats, scaling, and offsets on your specific target vehicle and model year using CAN sniffing tools.** Variations can exist.

### 2. General Notes

*   **Endianness:** Multi-byte values like speed and odometer are generally treated as **Big Endian** in the source repository (e.g., using helper functions like `get_be16`, `get_be24`).
*   **Target Bus:** Assumed to be CAN Comfort (125 kbps) unless otherwise specified.


## PSA PF2 generic CAN Messages (Derived from PSACANBridge & PSACAN.md)

### 3. CAN Message Details

---

**CAN ID:** `0x0B6`

*   **Source:** BSI / Engine ECU (Inferred)
*   **Period:** ~50ms (Estimated)
*   **Description:** Contains Engine RPM and potentially Vehicle Speed (Note: Speed is also reported in `0x14C`/`0x28C`).
*   **Signals:**
    *   **Engine RPM:**
        *   Bytes: `5`, `6`
        *   Decoding: `rpm = get_be16(&data[5]) / 4;` (Read bytes 5 & 6 as Big Endian uint16, divide by 4)
        *   Units: RPM
    *   **Vehicle Speed (Alternative Source):**
        *   Bytes: `3`, `4`
        *   Decoding: `speed = get_be16(&data[3]) * 0.01;` (Read bytes 3 & 4 as Big Endian uint16, multiply by 0.01)
        *   Units: km/h
    *   **Notes:** The `PSACANBridge` code *does not* decode ignition state or coolant temperature from this message ID.

---

**CAN ID:** `0x0E1`

*   **Source:** Parktronic ECU
*   **Period:** ~100ms (Estimated)
*   **Description:** Parking sensor distances and status.
*   **Signals:**
    *   **Rear Left Sensor Distance:**
        *   Bytes: `2`
        *   Bits/Mask: `0xE0` (Bits 7-5)
        *   Decoding: `(data[2] >> 5) & 0x07`
        *   Range: 0 (Closest) to 7 (Farthest/Inactive)
    *   **Rear Left Middle Sensor Distance:**
        *   Bytes: `2`
        *   Bits/Mask: `0x1C` (Bits 4-2)
        *   Decoding: `(data[2] >> 2) & 0x07`
        *   Range: 0-7
    *   **Rear Right Middle Sensor Distance:**
        *   Bytes: `2`, `3`
        *   Decoding: `((data[2] << 1) & 0x06) | ((data[3] >> 7) & 0x01)` (Combines bits 1-0 from Byte 2 and bit 7 from Byte 3)
        *   Range: 0-7
    *   **Rear Right Sensor Distance:**
        *   Bytes: `3`
        *   Bits/Mask: `0x70` (Bits 6-4)
        *   Decoding: `(data[3] >> 4) & 0x07`
        *   Range: 0-7
    *   **Front Left Sensor Distance:**
        *   Bytes: `3`
        *   Bits/Mask: `0x0E` (Bits 3-1)
        *   Decoding: `(data[3] >> 1) & 0x07`
        *   Range: 0-7
    *   **Front Left Middle Sensor Distance:**
        *   Bytes: `3`, `4`
        *   Decoding: `((data[3] << 2) & 0x04) | ((data[4] >> 6) & 0x03)` (Combines bit 0 from Byte 3 and bits 7-6 from Byte 4)
        *   Range: 0-7
    *   **Front Right Middle Sensor Distance:**
        *   Bytes: `4`
        *   Bits/Mask: `0x38` (Bits 5-3)
        *   Decoding: `(data[4] >> 3) & 0x07`
        *   Range: 0-7
    *   **Front Right Sensor Distance:**
        *   Bytes: `4`
        *   Bits/Mask: `0x07` (Bits 2-0)
        *   Decoding: `data[4] & 0x07`
        *   Range: 0-7
    *   **Parktronic Display Active:**
        *   Bytes: `5`
        *   Bits/Mask: `0x01` (Bit 0)
        *   States: `1`: Display is active, `0`: Display is inactive
    *   **Active Sensors (Front/Rear/Both):**
        *   Bytes: `1`
        *   Decoding: Based on bits 4, 5, 6.
            *   `data[1] & 0x10` (Bit 4): Front sensors active
            *   `data[1] & 0x40` (Bit 6): Rear sensors active
            *   `(data[1] & 0x10) && (data[1] & 0x40)` or potentially `data[1] & 0x20` (Bit 5): Both active (Needs verification)

---

**CAN ID:** `0x0F6`

*   **Source:** BSI
*   **Period:** ~100ms (Estimated)
*   **Description:** Turn indicators, reverse light, temperatures.
*   **Signals:**
    *   **Left Turn Indicator:**
        *   Bytes: `0`
        *   Bits/Mask: `0x01` (Bit 0)
        *   States: `1`: On, `0`: Off
    *   **Right Turn Indicator:**
        *   Bytes: `0`
        *   Bits/Mask: `0x02` (Bit 1)
        *   States: `1`: On, `0`: Off
    *   **Reverse Gear Light:**
        *   Bytes: `0`
        *   Bits/Mask: `0x04` (Bit 2)
        *   States: `1`: On, `0`: Off (Useful for `REAR` signal)
    *   **Ambient Temperature:**
        *   Bytes: `3`
        *   Decoding: `temp = (float)data[3] * 0.5 - 40.0;`
        *   Units: °C
    *   **Coolant Temperature:**
        *   Bytes: `5`
        *   Decoding: `temp = (float)data[5] - 40.0;`
        *   Units: °C

---

**CAN ID:** `0x128`

*   **Source:** BSI
*   **Period:** ~100ms (Estimated)
*   **Description:** Dashboard status lights (Parking Brake, Seatbelt, Doors, Lights).
*   **Signals:**
    *   **Driver Seatbelt Warning:** Byte 7, Bit 7 (`data[7] & 0x80`)
    *   **Parking Brake Light:** Byte 7, Bit 6 (`data[7] & 0x40`)
    *   **Door/Trunk Open Indicator:** Byte 6, Bit 5 (`data[6] & 0x20`)
    *   **Sidelights (Parking Lights) On:** Byte 5, Bit 7 (`data[5] & 0x80`)
    *   **Low Beam On:** Byte 5, Bit 6 (`data[5] & 0x40`)
    *   **High Beam On:** Byte 5, Bit 5 (`data[5] & 0x20`)
    *   **Front Fog Lights On:** Byte 5, Bit 4 (`data[5] & 0x10`)
    *   **Rear Fog Light On:** Byte 5, Bit 3 (`data[5] & 0x08`)
    *   **Right Turn Signal Indicator:** Byte 5, Bit 2 (`data[5] & 0x04`)
    *   **Left Turn Signal Indicator:** Byte 5, Bit 1 (`data[5] & 0x02`)
    *   **Low Fuel Warning:** Byte 4, Bit 0 (`data[4] & 0x01`)
    *   **Dashboard Backlight Brightness:** Bytes 0-1 (Raw value - `get_be16(&data[0])`) - *Note: `0x036` might be the primary source for dimming control.*

---

**CAN ID:** `0x131`

*   **Source:** BSI (Inferred)
*   **Period:** ~100ms (Estimated)
*   **Description:** Door status and fuel level calculation data.
*   **Signals:**
    *   **Door Front Left (FL):** Byte 1, Bit 0 (`data[1] & 0x01`) - `1`: Open
    *   **Door Front Right (FR):** Byte 1, Bit 1 (`data[1] & 0x02`) - `1`: Open
    *   **Door Rear Left (RL):** Byte 1, Bit 2 (`data[1] & 0x04`) - `1`: Open
    *   **Door Rear Right (RR):** Byte 1, Bit 3 (`data[1] & 0x08`) - `1`: Open
    *   **Bonnet (Hood):** Byte 1, Bit 4 (`data[1] & 0x10`) - `1`: Open
    *   **Tailgate (Trunk):** Byte 1, Bit 5 (`data[1] & 0x20`) - `1`: Open
    *   **Fuel Level (Raw - Use with 0x161):**
        *   Byte 0: Raw fuel level reading (potentially).
        *   Byte 3: Contains data for percentage calculation (see notes).
    *   **Notes:** Fuel level percentage calculation requires data from ID `0x161` (Byte 3) as well:
        ```c
        // Assuming 'data' is from 0x131 and 'data_161' is from 0x161
        fuel_level = (data_161[3] >> 2) & 0x3F; // Extract 6 bits from 0x161 byte 3
        max_fuel = (data_161[3] >> 1) & 0x7F;   // Extract 7 bits from 0x161 byte 3
        if (max_fuel != 0) {
            fuel_percent = (uint8_t)(((uint32_t)fuel_level * 100) / max_fuel);
        }
        ```

---

**CAN ID:** `0x14C` (and potentially duplicated by `0x28C`)

*   **Source:** BSI / ABS (Inferred)
*   **Period:** ~50ms (Estimated)
*   **Description:** Vehicle speed and odometer.
*   **Signals:**
    *   **Vehicle Speed:**
        *   Bytes: `0`, `1`
        *   Decoding: `speed = get_be16(data) * 0.01;` (Big Endian)
        *   Units: km/h
    *   **Odometer:**
        *   Bytes: `1`, `2`, `3`
        *   Decoding: `odometer = get_be24(&data[1]);` (Assumes `get_be24` reads Bytes 1, 2, 3 as Big Endian)
        *   Units: km

---

**CAN ID:** `0x161`

*   **Source:** BSI / Engine ECU (Inferred)
*   **Period:** ~1000ms (Estimated)
*   **Description:** Oil temperature and fuel level calculation details.
*   **Signals:**
    *   **Oil Temperature:**
        *   Bytes: `2`
        *   Decoding: `oil_temp = data[2] + 40;`
        *   Units: °C
    *   **Fuel Level Calculation Data:**
        *   Bytes: `3`
        *   Decoding: Used in conjunction with `0x131` for fuel percentage calculation (see `0x131` notes).
            *   `fuel_level = (data[3] >> 2) & 0x3F;`
            *   `max_fuel = (data[3] >> 1) & 0x7F;`

---

**CAN ID:** `0x168`

*   **Source:** BSI / Sensor (Inferred)
*   **Period:** ~1000ms (Estimated)
*   **Description:** Outside temperature and battery voltage.
*   **Signals:**
    *   **Outside Temperature:**
        *   Bytes: `0`
        *   Decoding: `temp = (float)data[0] * 0.5 - 40.0;`
        *   Units: °C
    *   **Battery Voltage:**
        *   Bytes: `1`
        *   Decoding: `voltage = (float)data[1] * 0.05 + 5.0;`
        *   Units: V

---

**CAN ID:** `0x1D0`

*   **Source:** Climate Control ECU
*   **Period:** ~100ms (Estimated)
*   **Description:** Climate control status information.
*   **Signals:**
    *   **Fan Speed:** Byte 5, Bits 2-0 (`data[5] & 0x07`) - Range 0-7.
    *   **Air Direction (Windshield):** Byte 4, Bit 6 (`data[4] & 0x40`) - `1`: On
    *   **Air Direction (Face/Middle):** Byte 4, Bit 5 (`data[4] & 0x20`) - `1`: On
    *   **Air Direction (Floor):** Byte 4, Bit 4 (`data[4] & 0x10`) - `1`: On
    *   **Air Recirculation:** Byte 3, Bit 7 (`data[3] & 0x80`) - `1`: On
    *   **A/C Compressor Active:** Byte 3, Bit 5 (`data[3] & 0x20`) - `1`: On
    *   **Left Zone Temperature:** Byte 1 - Raw value, needs mapping (e.g., `(data[1] * 0.5) + 14.0`)
    *   **Right Zone Temperature:** Byte 0 - Raw value, needs mapping (e.g., `(data[0] * 0.5) + 14.0`)

---

**CAN ID:** `0x21F`

*   **Source:** Steering Wheel Controls Module
*   **Period:** Event-driven (on button press/release/scroll)
*   **Description:** Steering wheel button presses and scroll wheel activity.
*   **Signals:** (Value is 1 when pressed/active, 0 when released)
    *   **Volume Up:** Byte 3, Bit 3 (`data[3] & 0x08`)
    *   **Volume Down:** Byte 3, Bit 2 (`data[3] & 0x04`)
    *   **Seek Forward / Next:** Byte 7, Bit 7 (`data[7] & 0x80`)
    *   **Seek Backward / Previous:** Byte 7, Bit 6 (`data[7] & 0x40`)
    *   **Source:** Byte 3, Bit 1 (`data[3] & 0x02`)
    *   **Scroll Wheel:** Byte 0 - Value changes during scrolling (signed?).

---

**CAN ID:** `0x2B6`, `0x336`, `0x3B6`

*   **Source:** BSI
*   **Period:** Infrequent / On Request?
*   **Description:** Vehicle Identification Number (VIN), split across messages.
*   **Signals:**
    *   `0x336`: Bytes 0-2 contain ASCII characters 1-3 of VIN.
    *   `0x3B6`: Bytes 0-5 contain ASCII characters 4-9 of VIN.
    *   `0x2B6`: Bytes 0-7 contain ASCII characters 10-17 of VIN.

---

**CAN ID:** `0x3E5`

*   **Source:** Radio / Multimedia Panel
*   **Period:** Event-driven
*   **Description:** Radio/Head Unit physical button presses.
*   **Signals:** (Value is 1 when pressed, 0 when released - check code for exact bits)
    *   Menu Button
    *   Tel (Phone) Button
    *   Clim (Climate) Button
    *   Trip Button
    *   Mode Button
    *   Audio Button
    *   OK Button
    *   Esc Button
    *   Dark Button
    *   Up/Down/Left/Right Buttons

---

## PSA CAN Bus CD Changer Emulation (Based on PSACANBridge)

The `PSACANBridge` project includes logic to emulate a factory PSA CD changer (or potentially a specific aftermarket interface like a Yatour YT-M06, given the message naming). This allows the factory head unit (Radio) to control an alternative audio source (in the case of `PSACANBridge`, often audio playback managed by the connected Raspberry Pi) using the standard CD/CDC controls and display track/status information on the factory display.

### 1. Core Principles

The emulation works on two main fronts:

1.  **Receiving Commands:** The emulator (the `PSACANBridge` firmware) listens for specific CAN messages sent *by the factory Radio* when the user interacts with the CD changer controls (e.g., Next Track, Change Disk, Play/Pause).
2.  **Sending Status & Information:** The emulator actively sends CAN messages *to the CAN bus* (targeting the Radio and/or Display) to report its current status, the "disk" and track number being played, playback time, and track metadata (if available). This keeps the factory displays updated.
3.  **State Management:** The emulator firmware must maintain an internal state representing the virtual CD changer (current disk, current track, playback status, time elapsed, total tracks, repeat/random modes, etc.).

### 2. Key CAN IDs Involved

*(IDs derived from `PSACAN.md` within the PSACANBridge context)*

**A. Commands Sent by Radio -> Received by Emulator:**

*   **`0x131` (CD Changer Command):** This is the primary message the factory Radio uses to control the CD changer/emulator.
    *   **Byte 7, Bit 7 (`E`):** Enable/Disable Changer (Select/Deselect CDC source)
    *   **Byte 7, Bit 5 (`B`):** Single Track Back (Previous)
    *   **Byte 7, Bit 4 (`F`):** Single Track Forward (Next)
    *   **Byte 6, Bit 7 (`X`):** Fast Forward / Rewind (Often requires holding Next/Prev)
    *   **Byte 6, Bit 5 (`I`):** Track Intro Mode Toggle
    *   **Byte 6, Bit 4 (`P`):** Play/Pause Toggle
    *   **Byte 5, Bit 5 (`R`):** Repeat Mode Toggle
    *   **Byte 4, Bit 5 (`R`):** Random Mode Toggle
    *   **Bytes 1-2 (`DDDDDDDD`):** Go to Disk N (Selects the virtual "disk")
    *   **Byte 0 (`TTTTTTTT`):** Go to Track N (Selects the track number)
    *   **Emulator Action:** The firmware needs a handler for `0x131` to parse these commands and trigger corresponding actions on its audio source (e.g., tell the RPi/Android HU to skip track, pause, change playlist/folder).

**B. Status/Info Sent by Emulator -> Received by Radio/Display:**

*   **`0x0E2` (Yatour Current Disk Number):** (Likely Yatour specific)
    *   **Byte 7, Bits 3-0 (`DDDD`):** Reports the currently active virtual disk number (1-15).
    *   **Emulator Action:** Send periodically or on change to update the display.
*   **`0x162` (Yatour Current Disk Status):** (Likely Yatour specific)
    *   **Byte 7, Bit 7 (`E`):** Radio Power Status (Emulator might reflect if it's active).
    *   **Byte 6, Bit 7 (`L`):** Disk Change in Progress flag (Set during virtual disk change).
    *   **Bytes 2-4, Bits (`SSS`):** Status Icon (Play=0, Pause=1, Stop=2, etc. - see `PSACAN.md`).
    *   **Byte 1, Bits 3-0 (`DDDD`):** Current Disk Number (redundant with `0x0E2`?).
    *   **Byte 0:** Fixed value (`0x06`) indicating CD changer source.
    *   **Emulator Action:** Send periodically to show playback status and disk number.
*   **`0x1A0` (Yatour Status Flags):** (Likely Yatour specific)
    *   **Byte 5, Bit 7 (`I`):** Intro Mode Active flag.
    *   **Byte 4, Bit 7 (`R`):** Random Mode Active flag.
    *   **Byte 3, Bit 7 (`R`):** Repeat Mode Active flag.
    *   **Emulator Action:** Send when these modes change based on `0x131` commands or internal state.
*   **`0x1A2` (Yatour Tracks Count):** (Likely Yatour specific)
    *   **Byte 7 (`CCCCCCCC`):** Total number of tracks on the current virtual disk.
    *   **Emulator Action:** Send when the virtual disk changes.
*   **`0x1E2` (Yatour Current Track Time):** (Likely Yatour specific)
    *   **Byte 7 (`TTTTTTTT`):** Current Track Number.
    *   **Byte 4 (`M|MMMMMMMM`):** Current Track Minute.
    *   **Byte 3 (`SSSSSSS`):** Current Track Second.
    *   **Bytes 5, 6, 0:** Indicate track switching status (`FF FF`, `80`).
    *   **Emulator Action:** Send frequently (e.g., every second) during playback, updating minutes/seconds based on an internal timer. Send switching status during track changes.
*   **`0x325` (CD Tray Information):**
    *   **Byte 4, Bit 6 (`I`):** Disk Inserting/Ejecting flag.
    *   **Byte 4, Bit 5 (`R`):** Disk Ready flag.
    *   **Byte 4, Bit 4 (`P`):** Disk Present flag.
    *   **Emulator Action:** Send based on the virtual state (e.g., always "Present" and "Ready" when active).
*   **`0x365` (CD Disk Information):**
    *   **Byte 7 (`NNNNNNNN`):** Number of Tracks on current disk.
    *   **Bytes 5-6:** Total Disk Time (Minutes).
    *   **Bytes 3-4:** Total Disk Time (Seconds).
    *   **Byte 0, Bit 7 (`M`):** CD Type (1 = MP3, 0 = Audio CD).
    *   **Emulator Action:** Send when virtual disk changes, reporting track count and potentially type (e.g., always MP3). Total time might be estimated or fixed.
*   **`0x3A5` (Current CD Track Info):**
    *   **Byte 7 (`NNNNNNNN`):** Current Track Number.
    *   **Bytes 6-5:** Total Track Time (Minutes, Seconds).
    *   **Bytes 4-3:** Current Track Time (Minutes, Seconds).
    *   **Emulator Action:** Send frequently during playback, similar to `0x1E2`, providing current time and potentially total track time if known.

*   **`0x0A4` / `0x125` (Track Text / Lists):** These messages use the ISO 15765-2 multi-frame protocol to send longer text strings (like track titles/artists or track lists). Implementing this requires handling the multi-frame protocol (segmentation, flow control).
    *   **Emulator Action:** If the audio source provides metadata, the emulator needs to format it according to these message structures and handle the multi-frame transmission sequence. This adds significant complexity.

**C. Related Status Messages (Used by Emulator):**

*   **`0x165` (Radio Status):** The emulator monitors this message sent *by the Radio*.
    *   **Bytes 2-4, Bits (`SSS`):** Indicates the currently selected audio source on the radio. The emulator should only become "active" (respond to `0x131`, send status) when the Radio selects the CD Changer source.
    *   **Byte 4, Bit 7 (`C`):** Indicates if the Radio *thinks* a CD changer is available. The emulator might need to send specific initialization messages (not detailed here) to make the Radio set this flag.

### 3. Relevance to `canbox-renode` Project

*   **Out of Scope (Initially):** Full CD changer emulation is likely **beyond the scope** of the primary goal of creating a basic CAN *proxy* for essential signals (IGN, REAR, ILLUM, Steering Wheel Buttons, etc.) for an Android Head Unit.
*   **Complexity:** Implementing CD changer emulation adds significant complexity:
    *   Requires handling many more CAN IDs.
    *   Requires robust internal state management (disk, track, time, modes).
    *   Requires frequent timed transmission of status messages.
    *   Implementing text/list transfer (`0x0A4`, `0x125`) involves the ISO 15765-2 protocol.
*   **Potential Adaptation:** *If* the Android Head Unit being used is designed to *replace* the factory radio but still *expects* to receive factory-style CD changer CAN messages (less common, but possible for some integrated units), then this logic could be adapted. The `canbox` would need to:
    1.  Receive playback status/commands from the Android HU (likely via its specific serial protocol, e.g., Raise/Hiworld).
    2.  Translate these into the internal state of the virtual CD changer.
    3.  Generate and send the corresponding PSA CAN messages (`0x162`, `0x1A0`, `0x1E2`, etc.) onto the vehicle's CAN bus.
    4.  Receive commands via `0x131` (if the HU sends them for some reason, maybe via steering wheel pass-through) and translate them back to the HU's serial protocol.

**Conclusion:**

The CD changer emulation described by the `PSACANBridge` project involves receiving commands on `0x131` and sending various status/time/track messages (like `0x162`, `0x1A0`, `0x1E2`, `0x3A5`, etc.) to mimic a physical changer for the factory radio/display. While the decoding information is valuable, implementing this full emulation within the `canbox-renode` firmware adds substantial complexity and is likely unnecessary for the primary goal of interfacing basic vehicle signals with a typical aftermarket Android Head Unit using protocols like Raise or Hiworld.

## PSA CAN Bus Radio Text / RDS (Radio Data System) (Based on PSACANBridge)


This functionality allows the factory head unit (Radio) to send information about the currently tuned station (Frequency, Band, Station Name (PS), Program Type (PTY), Traffic Announcements (TA), and Radio Text (RT)) over the CAN bus for display on the vehicle's multi-function display.

When building a `canbox` or other interface, you might want to either:

a.  **Listen** to these messages from the factory radio (if it's still present and connected to CAN) and relay the information to the Android head unit.
b.  **Generate** these messages if your `canbox` + Android head unit setup is fully *replacing* the factory radio's role on the CAN bus, making the Android HU's radio tuner appear like the factory one to other car modules (like the dashboard display). This is much more complex.

The `canbox-renode` project primarily focuses on **listening** to CAN and **proxying** to the Android HU, so we'll describe the messages from that perspective.

### 1. Core Principles of RDS on PSA CAN Bus (PF2 Era)

1.  **Tuning:** The factory Radio tunes to a station.
2.  **Basic Info Broadcast:** The Radio sends the current frequency and band (`0x225`).
3.  **RDS Signal Detection:** The Radio attempts to decode the RDS data stream embedded in the FM signal.
4.  **Status Broadcast:** Once RDS is detected, the Radio sends status updates (`0x265`, `0x1E0`) indicating RDS availability, Program Type (PTY), Traffic Announcement (TA) status, etc.
5.  **Text Broadcast:**
    *   **PS Name (Program Service):** The 8-character station name (e.g., "BBC R1") is broadcast. The exact ID for this in the `PSACANBridge` context isn't explicitly defined for `0x0A4` but it's a fundamental RDS feature. It might be sent via `0x0A4` using a specific page number or another dedicated ID (requires sniffing confirmation).
    *   **RT (Radio Text):** Longer messages (up to 64 characters, e.g., song title/artist) are sent using the multi-frame ISO 15765-2 protocol, typically via CAN ID `0x0A4`.
6.  **Display Update:** The vehicle's multi-function display listens to these messages and updates accordingly.

### 2. Key CAN IDs Involved (from `PSACANBridge` / `PSACAN.md`)

*   **`0x165` (Radio Status):**
    *   Indicates if the selected audio source (`SSS` bits) is actually the Tuner (FM/AM). The `canbox` should check this before interpreting radio-specific messages.
*   **`0x1E0` (Radio Control Flags):**
    *   Shows if features like RDS Alternative Frequencies (`A` bit), REG mode (`G` bit), or Radio Text display (`T` bit) are enabled *on the radio side*.
*   **`0x225` (Radio Frequency & Band):**
    *   Crucial for displaying the tuned station.
    *   Bytes 0-1: Contain the raw frequency value, needing calculation based on the band.
        *   FM: `Freq = (RawValue * 0.05) + 87.5` (MHz) (Derived from common PSA implementations)
        *   AM/LW: Different scaling needed.
    *   Byte 0-2 (B3/B5) or 2-4 (B7): Indicate the Band (FM1, FM2, FMAST, AM, LW).
*   **`0x265` (RDS Data):**
    *   Provides real-time RDS status.
    *   Bit 7 (`S`): RDS Signal Available/Activated.
    *   Bit 6 (`F`): RDS Data Found/Decoded successfully.
    *   Bit 5 (`T`): TA (Traffic Announcement) currently active.
    *   Bit 4 (`F`): TA signal available from the station.
    *   Bit 3 (`R`): Traffic Info Flag (General).
    *   Bit 1 (`P`/`M`): PTY (Program Type) flags.
    *   Byte 0 (`CCCCCCCC` / `PPPPPPPP`): Current station's PTY code or selected PTY code in menu.
*   **`0x0A4` (Current CD Track / Radio Text - ISO 15765-2):**
    *   **Multi-Frame Protocol:** This message is used to send longer strings that don't fit in a single 8-byte CAN frame. It requires assembling multiple CAN frames according to the ISO 15765-2 standard (which involves flow control frames).
    *   **Radio Text (RT):** When used for Radio Text, the documentation indicates it uses **Page 1 (`PPPP` bits = 1)**. The `TEXT_DATA` field contains segments of the ASCII encoded Radio Text message (up to 64 characters).
    *   **Station Name (PS)?** It's *possible* this ID is also used for the 8-character PS Name, perhaps on a different page number or with a specific format in the first frame. **This needs verification via CAN sniffing.** If PS Name isn't sent via `0x0A4`, it must be on a different, yet-to-be-identified ID for PF2.

### 3. Relevance to `canbox-renode` Project (Peugeot 407)

*   **Listening Role:** Your `canbox` will primarily act as a listener if the factory radio is still involved or if you are purely monitoring.
*   **Data Extraction:** To display radio info on the Android HU:
    1.  Your `peugeot_407.c` needs handlers for `0x165`, `0x1E0`, `0x225`, and `0x265`.
    2.  Decode the frequency, band, and status flags from these messages.
    3.  **Crucially:** If you want Radio Text, you need to implement an **ISO 15765-2 parser** within your firmware (or potentially in the Android HU if you pass raw multi-frame data) to listen for `0x0A4` (Page 1), handle the multi-frame sequence, and reconstruct the full text string. This is non-trivial.
    4.  Identify how the **PS Name** is transmitted (sniffing likely required) and add a handler for it.
*   **Data Forwarding:** The extracted and parsed information (Frequency, Band, RDS flags, PS Name, reconstructed Radio Text) needs to be formatted according to the chosen Android Head Unit protocol (e.g., Hiworld PSA) and sent over the serial link. Check the Hiworld documentation (`input.md`) for specific ComIDs related to radio info (e.g., `0xA2`, `0xA3`).

**Conclusion:**

Decoding basic RDS status (like frequency, band, TA status) from messages like `0x225` and `0x265` is relatively straightforward. However, displaying the dynamic **Station Name (PS)** and especially **Radio Text (RT)** requires significantly more effort due to the likely use of the **ISO 15765-2 multi-frame protocol** for `0x0A4` and the need to pinpoint the exact mechanism for PS name transmission. Implementing a full ISO-TP stack within the `canbox` firmware adds considerable complexity.


## PSA PF2 Platform Trip Computer CAN Messages (Derived from PSACANBridge & PSACAN.md)

### 1. Introduction

This section focuses on the CAN bus messages related to the Trip Computer functionality found on PSA PF2 platform vehicles (like the Peugeot 407). The information is primarily drawn from `PSACAN.md` and corroborated by the likely implementation patterns within the `PSACANBridge` project.

The goal is to identify and decode messages from the vehicle's CAN Comfort bus (125 kbps) that contain trip data, such as fuel consumption, range, average speed, and distance traveled. This data can then be relayed by the `canbox` firmware to the Android head unit using the appropriate head unit protocol (e.g., Hiworld PSA ComIDs `0x13`, `0x14`, `0x15`).

**Verification is essential.** While these IDs and formats are common, they must be confirmed on the specific target vehicle (Peugeot 407 2008MY) using CAN sniffing.

### 2. Key CAN IDs and Data

**CAN ID: `0x221` (Trip Computer Information - Page 0/Instantaneous)**

*   **Source:** BSI (Confirmed by `PSACAN.md`)
*   **Period:** Variable, likely updates frequently during driving (e.g., 500ms - 1000ms).
*   **Description:** Provides core instantaneous consumption and range information. Corresponds closely to data needed for Hiworld ComID `0x13`.
*   **Signals (Based on `PSACAN.md`):**
    *   **Instantaneous Fuel Consumption (L/100km):**
        *   Bytes: `4`, `5`
        *   Decoding: `value = get_be16(&data[4]);` (Big Endian assumed). Scaling factor is needed (e.g., divide by 10 or 100?). `PSACAN.md` only gives raw location. **Verification Required.**
        *   Invalid Flag: Byte 7, Bit 0 (`data[7] & 0x01`) - `1`: Display "--.-".
    *   **Range on Current Fuel (Distance to Empty - DTE):**
        *   Bytes: `2`, `3`
        *   Decoding: `range = get_be16(&data[2]);` (Big Endian assumed).
        *   Units: km (Likely, requires verification).
        *   Invalid Flag: Byte 6, Bit 0 (`data[6] & 0x01`) - `1`: Display "----".
    *   **(Potentially) Range to Empty Tank (Less Common):**
        *   Bytes: `0`, `1`
        *   Decoding: `range = get_be16(&data[0]);` (Big Endian assumed).
        *   Units: km (Likely, requires verification).
    *   **Trip Reset Button Flag:**
        *   Bytes: `4` (Seems wrong in `PSACAN.md`, maybe Byte 7?)
        *   Bits/Mask: `0x80` ? (`T` flag in `PSACAN.md`) - Indicates press of the trip computer stalk button. Less relevant for display.

**CAN IDs for Trip 1 & Trip 2 Data (Average Speed, Distance)**

*   **Source:** BSI
*   **IDs on PF2 (Peugeot 407):** **UNKNOWN - Requires Sniffing.** `PSACAN.md` *only* explicitly lists `0x261` and `0x2A1` for the *newer C4 (B7)* platform for this data.
*   **Likely Data Types (Based on B7 and Hiworld `0x14`/`0x15`):** We expect to find messages containing:
    *   **Average Fuel Consumption (Trip 1 & Trip 2):** L/100km.
    *   **Average Speed (Trip 1 & Trip 2):** km/h.
    *   **Distance Traveled (Trip 1 & Trip 2):** km.
    *   **Trip Time (Trip 1 & Trip 2):** Hours/Minutes (Potentially calculated by HU or derived).
*   **Action Needed:** Sniff the CAN Comfort bus on the Peugeot 407 while:
    1.  Driving to accumulate trip data.
    2.  Switching between Trip 1, Trip 2, and Instantaneous views on the factory dashboard display (using the stalk button).
    3.  Resetting Trip 1 and Trip 2.
    *   Look for changing CAN messages that correlate with the displayed values for average speed, distance, and average consumption.

### 3. Head Unit Control (Hiworld ComID `0x1B`)

*   **Direction:** Head Unit -> CANbox
*   **Description:** The head unit *may* send this command to request a reset of Trip 1 or Trip 2 data.
*   **`canbox` Action:** If this functionality is desired, the `canbox_hiworld_psa_cmd_process` function needs to parse ComID `0x1B`. However, the `canbox` firmware itself cannot typically reset the vehicle's BSI trip computer directly just by listening. This command would likely only be relevant if the `canbox` were actively *replacing* the BSI's role for trip calculation (which is not the case). For a proxy, this command can likely be ignored or just ACK'd.

### 4. `canbox` Implementation Notes

1.  **Add Handler for `0x221`:** Implement `peugeot_407_ms_221_handler` in `peugeot_407.c` to decode instantaneous consumption and range, updating relevant fields (potentially new fields needed) in `carstate`. Remember to handle the "invalid" flags.
2.  **Add Placeholders/Search for Trip 1/2 IDs:** Add entries in `peugeot_407_ms[]` with placeholder handlers for the *expected* but unknown IDs carrying average speed/distance. Once sniffing reveals the IDs, implement the handlers.
3.  **Update `carstate`:** Consider adding members to `car_state_t` if they don't exist to store:
    *   `inst_consumption`
    *   `range`
    *   `avg_consumption1`, `avg_consumption2`
    *   `avg_speed1`, `avg_speed2`
    *   `trip_distance1`, `trip_distance2`
4.  **Map to Hiworld Protocol:** In `protocol/hiworldpsa.c`, modify/create functions to send ComIDs `0x13`, `0x14`, `0x15`. These functions will read the corresponding values from `carstate` (once populated by the CAN handlers) and format them according to the Hiworld data structure specified in `input.md`.

**Conclusion:**

Message `0x221` is the key starting point for instantaneous trip data on the PF2 platform. Finding the CAN messages for averaged trip data (Trip 1/Trip 2 average speed, distance, etc.) on the Peugeot 407 requires **dedicated CAN bus sniffing** while interacting with the trip computer display and reset functions. The Hiworld protocol provides a clear target for the *types* of data the head unit expects (`0x13`, `0x14`, `0x15`), guiding the reverse-engineering process.

## PSA PF2 Platform Audio Settings CAN Messages (Derived from PSACANBridge & PSACAN.md)

### 1. Introduction

This section describes the CAN bus message associated with adjusting audio settings like Bass, Treble, Balance, Fader, Loudness, and pre-defined equalizer/ambiance modes on PSA PF2 platform vehicles (e.g., Peugeot 407). The information is primarily sourced from `PSACAN.md` and corroborated by the handling within the `morcibacsi/PSACANBridge` project.

Understanding this message helps clarify how the factory system operates, but it's important to determine its relevance for a typical aftermarket Android head unit installation using a CANbox proxy.

### 2. Key CAN ID and Data

**CAN ID: `0x1E5` (Audio Settings)**

*   **Source:** Factory Radio / Head Unit (Confirmed by `PSACAN.md` and `PSACANBridge`)
*   **Destination:** Multi-Function Display (MFD) / BSI? (Primarily for display updates)
*   **Period:** ~500ms (According to `PSACAN.md`), and likely event-driven when settings are changed.
*   **Description:** This message reflects the current audio settings adjusted via the factory radio's controls. It includes both the setting value and flags indicating which audio menu is currently being displayed or adjusted on the factory MFD.
*   **Signals (Based on `PSACAN.md` and `PSACANBridge`):**

    | Byte   | Bit(s) | Name                  | Description & Value Mapping                                                                                                |
    | :----- | :----- | :-------------------- | :------------------------------------------------------------------------------------------------------------------------- |
    | **7**  | 7      | `S` Show Balance L/R  | 1: Balance L/R menu is active/shown on MFD.                                                                                |
    |        | 6..0   | `sssssss` Balance L/R | Value: `0x3F`=Center, `<0x3F`=Left (e.g., `0x36`=-9), `>0x3F`=Right (e.g., `0x48`=+9). See table in `PSACAN.md`.         |
    | **6**  | 7      | `F` Show Fader F/R    | 1: Fader F/R menu is active/shown on MFD.                                                                                  |
    |        | 6..0   | `fffffff` Fader F/R   | Value: `0x3F`=Center, `<0x3F`=Rear (e.g., `0x36`=-9), `>0x3F`=Front (e.g., `0x48`=+9). See table in `PSACAN.md`.         |
    | **5**  | 7      | `B` Show Bass         | 1: Bass menu is active/shown on MFD.                                                                                       |
    |        | 6..0   | `bbbbbbb` Bass        | Value: `0x40`=0, `<0x40`=Negative (e.g., `0x37`=-9), `>0x40`=Positive (e.g., `0x49`=+9). See table in `PSACAN.md`.     |
    | **4**  | 7      | `T` Show Treble       | 1: Treble menu is active/shown on MFD.                                                                                     |
    |        | 6..0   | `ttttttt` Treble      | Value: `0x40`=0, `<0x40`=Negative (e.g., `0x37`=-9), `>0x40`=Positive (e.g., `0x49`=+9). See table in `PSACAN.md`.     |
    | **3**  | 7      | `L` Show Loudness     | 1: Loudness menu is active/shown on MFD.                                                                                   |
    |        | 6      | `l` Loudness Status   | `1`: Loudness ON, `0`: Loudness OFF.                                                                                       |
    |        | 5      | `A` Show Auto Volume  | 1: Automatic Volume menu is active/shown on MFD.                                                                           |
    |        | 4..2   | `aaa` Auto Volume Lvl | Value: `0x0`=OFF, `0x1`..`0x5` = Level 1 to 5. (`PSACANBridge` maps this 0-5).                                           |
    | **2**  | 7      | `P` Show Ambiance     | 1: Musical Ambiance/EQ menu is active/shown on MFD.                                                                        |
    |        | 4..0   | `ppppp` Ambiance/EQ   | Value: `0x00`=None/Manual, `0x01`=Classical, `0x02`=Jazz/Blues, `0x03`=Pop/Rock, `0x04`=Vocal, `0x05`=Techno. See `PSACAN.md`. |

### 3. Relevance to `canbox-renode` Project (Peugeot 407)

*   **Source:** This message (`0x1E5`) originates from the **factory radio**.
*   **Typical Scenario:** In most aftermarket Android head unit installations, the factory radio is **removed**. Therefore, the source of the `0x1E5` message is gone.
*   **Android HU Responsibility:** The Android head unit has its own internal audio processing, equalizer, balance, and fader settings, managed through its own user interface and DSP. It does *not* typically rely on receiving CAN message `0x1E5` to set these parameters.
*   **`canbox` Role:** The `canbox` firmware acts as a proxy for essential *vehicle state* information (ignition, lights, doors, speed, SWC for volume/track *commands*, etc.) needed by the Android HU, translating them into the HU's specific serial protocol (like Hiworld PSA). It generally **does not** need to:
    *   **Listen** for `0x1E5` (as the source is usually removed).
    *   **Generate** `0x1E5` (as the Android HU manages its own settings).
*   **Hiworld PSA Protocol (`input.md`):** A review of the Hiworld PSA protocol document shows ComIDs for sending *basic* audio information *from* the HU (like radio frequency `0xA2`, CD info `0xA4`) but **no specific ComIDs for the CANbox to report Bass/Treble/Balance/Fader status *from* the car to the HU**, nor commands for the HU to *set* these parameters via the CANbox onto the CAN bus. Volume control is typically handled by separate steering wheel commands (like Hiworld `0x21` or `0x22`) translated from CAN.

**Conclusion:**

While CAN ID `0x1E5` provides detailed information about the factory radio's audio settings on the PF2 platform, it is **generally not relevant** for the standard `canbox-renode` proxy functionality when installing an aftermarket Android head unit that replaces the factory radio.

*   **No Need to Handle:** You likely **do not** need to add a handler for `0x1E5` in `cars/peugeot_407.c`.
*   **No Need to Transmit:** You likely **do not** need to add logic in `protocol/hiworldpsa.c` to send this specific audio settings information to the head unit.



Okay, here are the descriptions for the remaining CAN IDs mentioned in `PSACAN.md`, formatted similarly to the previous documentation entries.

---

## PSA PF2 Platform - Additional CAN Message Reference (Derived from PSACAN.md)

This section details additional CAN messages found in `PSACAN.md` that were not covered in the primary functional descriptions. These messages often relate to the factory display, factory radio controls, or specific informational/diagnostic functions. Their relevance to a standard aftermarket Android head unit installation via a CANbox proxy is often limited, as the factory components they interact with may be removed or bypassed.

**Verification is essential** if you intend to utilize data from these messages.

---

**CAN ID: `0x0DF` (Display Menu Control)**

*   **Source:** Factory Radio or Factory Display (Interacts with Display)
*   **Destination:** Factory Display
*   **Period:** ~500ms (According to `PSACAN.md`)
*   **Description:** Controls the behavior and status of menus on the factory Multi-Function Display (MFD).
*   **Signals (Based on `PSACAN.md`):**
    | Byte   | Bit(s) | Name                  | Description                                                                                                                          |
    | :----- | :----- | :-------------------- | :----------------------------------------------------------------------------------------------------------------------------------- |
    | **7**  | 7      | `M` Main Menu/Dark    | `1`: Main menu active, OK menu active, or Dark button short press. `0`: Otherwise.                                                    |
    | **0**  | 7      | `Z` Popup/Mode Status | `1`: A popup is active (e.g., Audio Menu, Track List, PTY List), or Dark mode is active, or Inverse mode is active. `0`: Otherwise. |
    | Others |        | Unknown/Variable      | Other bits appear to hold non-constant or undocumented values.                                                                       |
*   **Relevance to `canbox-renode`:** **Low.** This message controls the factory display's UI state. An aftermarket Android HU manages its own UI. The `canbox` typically doesn't need to interpret or generate this message.

---

**CAN ID: `0x122` (Universal Multiplexed Panel / FMUX)**

*   **Source:** FMUX (Front Multiplexing Unit - Physical radio/media control panel)
*   **Destination:** Factory Radio
*   **Period:** Event-driven (on button press/rotation)
*   **Description:** Reports button presses and knob rotations from the main physical radio/media control panel (the one often containing preset buttons, CD slot, volume knob, etc.).
*   **Signals (Based on `PSACAN.md` - indicates bit position for '1' when pressed):**
    *   Byte 7: Bit 7 (`A`)=`1`, Bit 6 (`B`)=`2`, Bit 5 (`C`)=`3`, Bit 4 (`D`)=`4`, Bit 3 (`E`)=`5`, Bit 2 (`F`)=`6`, Bit 1 (`G`)=`EJECT`, Bit 0 (`H`)=`ESC`
    *   Byte 6: Bit 7 (`I`)=`PAGE`, Bit 6 (`J`)=`TA INFO`, Bit 5 (`K`)=`SOURCE`, Bit 4 (`L`)=`MENU`, Bit 3 (`M`)=`LIST`, Bit 2 (`N`)=`EQUALIZER`, Bit 1 (`O`)=`BAND`, Bit 0 (`P`)=`LEFT Arrow`
    *   Byte 5: Bit 7 (`Q`)=`RIGHT Arrow`, Bit 6 (`R`)=`UP Arrow`, Bit 5 (`S`)=`DOWN Arrow`, Bit 4 (`T`)=`OK`
    *   Bytes 1-2: Left Control Knob Rotation (Signed value? CW increase?)
    *   Byte 0: Right Control Knob Rotation (Signed value? CW increase?)
*   **Relevance to `canbox-renode`:** **Low to Medium.** If the factory radio panel remains functional alongside the Android HU, and the HU needs to react to these specific physical buttons (beyond standard steering wheel controls), you *could* decode this message. However, most Android HUs rely on their own touchscreen and steering wheel inputs. The steering wheel controls (`0x21F`) are usually sufficient. The `canbox` doesn't typically need to *generate* these.

---

**CAN ID: `0x1A1` (BSI Informational Messages)**

*   **Source:** BSI
*   **Destination:** Factory Display
*   **Period:** Event-driven
*   **Description:** Displays various status and warning messages on the factory MFD.
*   **Signals (Based on `PSACAN.md`):**
    | Byte   | Bit(s) | Name         | Description                                                                                                                   |
    | :----- | :----- | :----------- | :---------------------------------------------------------------------------------------------------------------------------- |
    | **7**  | 7-0    | Window Ctrl  | Controls how the message is displayed (e.g., `0x82`=Show, `0x02`=Hide, `0x01`=Clear).                                          |
    | **6**  | 7-0    | Message Code | Identifies the specific message (e.g., `0x01`=Parking Assist Fault, `0x11`=Low Fuel, `0x1D`=Ice Alert). See table in `PSACAN.md`. |
    | **5**  | 7      | `Z` Show Flag| `1`: Show the message.                                                                                                        |
    | 0-4    |        | Parameters   | Optional additional data specific to the Message Code (often unused or `0x00`).                                                 |
*   **Relevance to `canbox-renode`:** **Medium.** While not essential for basic operation, decoding these messages could allow the `canbox` to:
    1.  Log vehicle warnings for diagnostic purposes.
    2.  Potentially forward critical warnings to the Android HU using the chosen serial protocol (if the protocol supports custom alerts, like Hiworld `0x42`). This would require mapping PSA message codes to the HU protocol's alert types.

---

**CAN ID: `0x1ED` (Display Conditioning Commands)**

*   **Source:** Factory Display (Sent when user changes settings via MFD)
*   **Destination:** BSI / Climate Control ECU?
*   **Period:** Event-driven
*   **Description:** Sends commands related to climate control display options set via the factory MFD.
*   **Signals (Based on `PSACAN.md`):**
    | Byte   | Bit(s) | Name        | Value (Hex) | Description                                  |
    | :----- | :----- | :---------- | :---------- | :------------------------------------------- |
    | **0**  | 7-0    | Command Val | `0x00`      | Default (No specific command)                |
    |        |        |             | `0x40`      | Simultaneous LH-RH Temp Control Enabled      |
    |        |        |             | `0x80`      | Simultaneous LH-RH Temp Control Disabled     |
    |        |        |             | `0xC1`      | A/C Deactivated                              |
*   **Relevance to `canbox-renode`:** **Low.** This message originates from the factory display controls. The `canbox` typically only needs to *read* climate status (`0x1D0`), not send display-related configuration commands *to* the climate system.

---

**CAN ID: `0x220` (Door Status - Alternative/Redundant)**

*   **Source:** BSI
*   **Destination:** Various ECUs needing door status
*   **Period:** Event-driven / Periodic? (~100ms Estimate)
*   **Description:** Reports the open/closed status of doors and trunk. Seems largely redundant with information in `0x131` for PF2 vehicles.
*   **Signals (Based on `PSACAN.md`):**
    | Byte   | Bit(s) | Name             | Description          |
    | :----- | :----- | :--------------- | :------------------- |
    | **0**  | 7      | Door Front Left  | `1`: Open, `0`: Closed |
    |        | 6      | Door Front Right | `1`: Open, `0`: Closed |
    |        | 5      | Door Rear Left   | `1`: Open, `0`: Closed |
    |        | 4      | Door Rear Right  | `1`: Open, `0`: Closed |
    |        | 3      | Door Trunk       | `1`: Open, `0`: Closed |
*   **Relevance to `canbox-renode`:** **Low to Medium.** Primarily useful for confirming which ID (`0x131` or `0x220`) is the main source for door status on your specific 407. Handling `0x131` is usually sufficient as it also contains bonnet status and fuel level data.

---

**CAN ID: `0x39B` (Set System Time)**

*   **Source:** Factory Display / RT6 Head Unit
*   **Destination:** BSI
*   **Period:** Event-driven (when user sets time/date)
*   **Description:** Command sent *from* the factory interface *to* the BSI to update the vehicle's internal clock.
*   **Signals (Based on `PSACAN.md` - Format matches `0x276` from B7):**
    | Byte   | Bit(s) | Name        | Description                |
    | :----- | :----- | :---------- | :------------------------- |
    | **7**  | 7      | `A` Format  | `1`: 12h, `0`: 24h         |
    | **6**  | 6-0    | `YYYYYYY` Year | Value + 2000 (Range 0-125) |
    | **4**  | 3-0    | `MMMM` Month | Value 1-12                 |
    | **3**  | 4-0    | `DDDDD` Day   | Value 1-31                 |
    | **2**  | 4-0    | `HHHHH` Hour  | Value 0-23                 |
    | **1**  | 5-0    | `ZZZZZZ` Min  | Value 0-59                 |
*   **Relevance to `canbox-renode`:** **Low.** The `canbox` is a listener/proxy. It doesn't set the car's time. The Android HU gets time from other sources (GPS/Network). If the HU's protocol supports sending time updates (like Hiworld `0xCB`), the `canbox` would need to *receive* that serial command and *potentially* generate this `0x39B` CAN message, but that's advanced functionality beyond the basic proxy role.

---

**CAN ID: `0x3F6` (Date and Time - Possible/Unclear)**

*   **Source:** Factory Radio
*   **Destination:** Factory Display
*   **Period:** Unknown
*   **Description:** `PSACAN.md` notes this potentially relates to time, but the format is unclear.
*   **Signals (Based on `PSACAN.md` - Speculative):**
    *   Bytes 0-2 (`XXXXXXXXXXXXXXX`): Suspected "Time in seconds".
    *   Byte 5, Bit 7 (`Z`): Time Format (`0`=12h, `1`=24h).
*   **Relevance to `canbox-renode`:** **Very Low.** Unclear purpose and likely originates from the factory radio, which is usually removed. Not needed for standard `canbox` functions.

---

**CAN ID: `0x51D` (Parktronic Status - Example Data)**

*   **Source:** Parktronic ECU
*   **Destination:** Unknown (Likely Display/Radio)
*   **Period:** Unknown
*   **Description:** `PSACAN.md` only provides raw data examples (`01 00 00 ...`, `05 00 53 ...`) without decoding the structure.
*   **Relevance to `canbox-renode`:** **None.** The decoding is provided by ID `0x0E1`. This ID seems redundant or preliminary in the source document.

---

**CAN ID: `0x5E0` (Radio Hardware/Software Info)**

*   **Source:** Factory Radio
*   **Destination:** Diagnostic Tool / BSI?
*   **Period:** Infrequent / On Request?
*   **Description:** Contains hardware/software version information for the factory radio unit.
*   **Signals (Based on `PSACAN.md`):**
    *   Bytes 3-4: Firmware Version (VV.vv format).
*   **Relevance to `canbox-renode`:** **None.** Specific to the factory radio.

---

**CAN ID: `0x5E5` (Display Hardware/Software Info)**

*   **Source:** Factory Display (MFD)
*   **Destination:** Diagnostic Tool / BSI?
*   **Period:** Infrequent / On Request?
*   **Description:** Contains hardware/software version information for the factory multi-function display.
*   **Signals (Based on `PSACAN.md`):**
    *   Bytes 3-4: Firmware Version (VV.vv format).
*   **Relevance to `canbox-renode`:** **None.** Specific to the factory display.

---


**Other IDs like `0x0A4`, `0x122`, `0x125`, `0x162`, `0x165`, `0x1A0`, `0x1A1`, `0x1A2`, `0x1A5`, `0x1E0`, `0x1E2`, `0x1E5`, `0x220`, `0x221`, `0x225`, `0x265`, `0x325`, `0x365`, `0x3A5` are present in `PSACANBridge` but are mostly related to 
- CD changer emulation, 
- radio text/RDS display, 
- specific trip computer displays, or 
- audio settings 
which might be lower priority for basic canbox functionality. Refer to `Src/psa.c` in the repository for their decoding if needed.**


Please describe also: 