# PSA PF2 Platform CAN Message Reference (Derived from PSACANBridge)

**Source Repository:** [https://github.com/morcibacsi/PSACANBridge](https://github.com/morcibacsi/PSACANBridge)

**`doc/psa_pf2_comfort.md` (Version 1.8 - Added Button Mapping & CDC Context)**

**Source Note:** This version incorporates information from `PSACAN.md`, `fazerxlo/simulator`, `kuba2k2/CDCEmu`, and analysis of logs from a Peugeot 407. It covers signals for basic CAN proxying and messages relevant to **CD Changer (CDC) Emulation**. Details may differ based on specific vehicle models, years, and equipment. **Always verify on your target system.**

### 1. Introduction

This document provides a reference for CAN bus messages relevant to PSA PF2 platform vehicles (e.g., Peugeot 407, Citroën C5 Mk1/Mk2, Peugeot 307/308 Mk1).

The primary focus is on messages typically found on the **CAN Comfort (CAN-INFO) bus** (usually **125 kbps**). This information is crucial for developing CAN interface devices like CAN boxes (proxies), custom integrations, and **CD Changer Emulators**.

### 2. General Notes

*   **Endianness:** Multi-byte values are generally treated as **Big Endian**.
*   **Target Bus:** Assumed to be CAN Comfort (125 kbps).
*   **Project Context:** Information is synthesized from multiple sources. Implementations may vary.

## PSA PF2 CAN Message Details

*(Messages grouped by primary function/source ECU)*

---

### BSI (Body Systems Interface) Originated Messages

**CAN ID:** `0x036`
*   **Source:** BSI
*   **Description:** Ignition mode, dashboard lighting status. Essential for ACC/IGN/ILLUM signals.
*   **Period:** ~100ms
*   **Signals (Verified on 407 Log/Consistent Interpretation):**
    | Byte | Bit(s)        | Description                                  | Proxy Use                          |
    | :--- | :------------ | :------------------------------------------- | :--------------------------------- |
    | 3    | Bit 5 (`L`)   | Dashboard Lighting Enabled (1=Enabled)       | Drive `ILLUM` pin state            |
    | 3    | Bits 3-0 (`l`) | Dashboard Lighting Brightness Level (0-15)   | Optional: Dimming control      |
    | 4    | Bits 1-0 (`P`) | Power Mode (Ignition/ACC state)            | Drive `ACC`/`IGN` pin state        |
    *   **Power Mode Values:** `00`: OFF, `01`: IGN_ON (Run), `03`: ACC_ON (Accessory)

---

**CAN ID:** `0x0B6`

*   **Source:** BSI / Engine ECU
*   **Description:** Engine RPM and potentially Vehicle Speed.
*   **Period:** ~50ms
*   **CDCEmu Function (Receiver):** `processFrame` (Reads RPM B5-6, Speed B3-4)
*   **Canbox Proxy Role (Receiver):** Use `peugeot_407_ms_0B6_engine_status_handler` (RPM B5-6, Coolant B5).
*   **Signals (Choose interpretation based on verification):**
    *   **Option A (CDCEmu/PSACANBridge):**
        *   Bytes 5-6: Engine RPM (`Raw / 4`)
        *   Bytes 3-4: Vehicle Speed (`Raw * 0.01` km/h)
    *   **Option B (Simulator):**
        *   Bytes 0-1: Engine RPM (`Raw * 0.1`)
        *   Bytes 2-3: Vehicle Speed (`Raw * 0.01` km/h)
    *   **Option C (Your 407 Handler):**
        *   Bytes 5-6: Engine RPM (`Raw / 4`)
        *   Byte 5: Coolant Temp (`Raw * 0.75 - 48`)
*   **Note:** Significant discrepancies exist. **Must verify for target vehicle.** Speed is reliably available on `0x14C`/`0x28C`.

---

**CAN ID:** `0x0E1`

*   **Source:** Parktronic ECU
*   **Description:** Parking sensor distances and status.
*   **Period:** ~100ms
*   **CDCEmu Function (Receiver):** Not explicitly handled in snippet, likely inferred via `0x0F6` reverse + other logic.
*   **Canbox Proxy Role (Receiver):** Use `peugeot_407_ms_0E1_parktronic_handler`.
*   **Signals (Based on `PSACANBridge`/407 Logs):**
    *   Byte 1: Active Sensors flags (Bit 4=Front, Bit 6=Rear)
    *   Byte 5, Bit 0: Parktronic Display Active flag (1=Active)
    *   Bytes 2, 3, 4: Contain packed sensor distances (6 Front + 6 Rear bits). Decode using bit shifts/masks. Range 0 (Closest) to 7 (Farthest/Inactive).

---

**CAN ID:** `0x0F6`
*   **Source:** BSI
*   **Description:** Turn indicators, Reverse Light status, Temperatures.
*   **Period:** ~100ms
*   **Signals (Conflicting Interpretations - VERIFY ON TARGET 407):**
    | Byte | Bit(s)        | Description                     | Proxy Use                     | Notes                                       |
    | :--- | :------------ | :------------------------------ | :---------------------------- | :------------------------------------------ |
    | 0    | Bit 3 (?)     | Reverse Gear Light (1=ON)     | Drive `REAR` pin state        | **Verify bit!** (Log=B0b3, CDCEmu=B7b7, PSB=B0b2) |
    | 0    | Bit 1         | Right Turn Indicator Light    | Optional: HU Display/Sound    |                                             |
    | 0    | Bit 0         | Left Turn Indicator Light     | Optional: HU Display/Sound    |                                             |
    | 3    | `TTTTTTTT`    | Outside Ambient Temperature     | Report via Serial (`0x36` RZC)  | `(Raw * 0.5) - 40` °C. Matches PSB/407 Hdlr. |
    | 1/5? | `CCCCCCCC`    | Engine Coolant Temperature    | Report via Serial             | Byte varies by source (`CDCEmu`=B1, `PSB`=B5). Use `0x0B6` instead? |
    | 7    | Bit 7         | Reverse Gear Light (CDCEmu Ver) | Drive `REAR` pin state        | Alternative interpretation.                 |
    | 5    | `TTTTTTTT`    | Ambient Temp (CDCEmu Ver)     | Report via Serial (`0x36` RZC)  | Different byte than PSB/407 Handler.        |

---

**CAN ID:** `0x128`
*   **Source:** BSI
*   **Description:** Dashboard status/indicator lights.
*   **Period:** ~100ms
*   **Signals:**
    | Byte | Bit(s) | Description                        | Proxy Use                            | Notes                                |
    | :--- | :----- | :--------------------------------- | :----------------------------------- | :----------------------------------- |
    | 7    | Bit 7  | Driver Seatbelt Warning Light    | Report via Serial (`0x38` RZC?)    | 1 = Warning/Unfastened           |
    | 7    | Bit 6  | Parking Brake Light              | Drive `PARK` pin, Report Serial     | 1 = On. Indicator light state only. |
    | 6    | Bit 5  | Door/Trunk Open Indicator Light  | Info only (Use `0x131` for state) |                                      |
    | 5    | Bit 7  | Sidelights (Parking Lights) On   | Report via Serial (`0x38` RZC)    |                                      |
    | 5    | Bit 6  | Low Beam (Near Lights) On      | Report via Serial (Not in `0x38`?) |                                      |
    | 5    | Bit 5  | High Beam On                     | Optional reporting                 |                                      |
    | 5    | Bit 4  | Front Fog Lights On              | Optional reporting                 |                                      |
    | 5    | Bit 3  | Rear Fog Light On                | Optional reporting                 |                                      |
    | 5    | Bit 2  | Right Turn Signal Indicator      | Optional reporting                 |                                      |
    | 5    | Bit 1  | Left Turn Signal Indicator       | Optional reporting                 |                                      |
    | 4    | Bit 0  | Low Fuel Warning Light           | Optional reporting                 |                                      |

---

**CAN ID:** `0x131`
*   **Source:** BSI (Door Status), Factory Radio (CDC Commands)
*   **Description:** Door status AND CD Changer commands.
*   **Period:** ~100ms (Doors), Event-driven (CDC Commands)
*   **Proxy Role:** Process Door status bits. Ignore CDC command bits.
*   **CDC Emulator Role:** Process CDC command bits.
*   **Signals:**
    *   **Doors (Consistent):** Byte 1, Bits 0-5 (FL, FR, RL, RR, Bonnet, Tailgate) (1=Open) -> Use for RZC `0x38`.
    *   **Fuel Info:** Byte 3 (Used with `0x161`) -> Use for RZC `0x42` (Fuel %).
    *   **CDC Commands (Ignored by Proxy, Used by CDC Emu):** See CDC Emulation section.
---

**CAN ID:** `0x14C` / `0x28C`

*   **Source:** BSI / ABS
*   **Description:** Vehicle speed and odometer.
*   **Period:** ~50ms
*   **CDCEmu Function (Receiver):** `processFrame` (Speed B0-1, Odo B1-3)
*   **Canbox Proxy Role (Receiver):** Use `peugeot_407_ms_28C_speed_odo_handler`.
*   **Signals (Consistent):**
    *   Bytes 0-1: Vehicle Speed (`Raw * 0.01` km/h, Big Endian)
    *   Bytes 1-3: Odometer (km, 24-bit Big Endian `[B1][B2][B3]`)

---

**CAN ID:** `0x161`
*   **Source:** BSI / Engine ECU
*   **Description:** Oil temperature and Fuel level calculation details.
*   **Period:** ~1000ms
*   **Signals (Consistent):**
    *   Byte 2: Oil Temperature -> Report via Serial (e.g., MQB `0x45`). (`Raw + 40` °C, `0xFF`=Invalid).
    *   Byte 3: Fuel Level Calculation Bits (`Level=(Raw>>2)&0x3F`, `Max=(Raw>>1)&0x7F`) -> Calculate % for Serial (e.g., MQB `0x42`).

---

**CAN ID:** `0x168`
*   **Source:** BSI / Sensor
*   **Description:** Outside temperature and Battery voltage.
*   **Period:** ~1000ms
*   **Signals (Consistent):**
    *   Byte 0: Outside Temperature -> Report via Serial (RZC `0x36`, MQB `0x42`/`0x45`). (`Raw * 0.5 - 40` °C, `0xFF`=Invalid).
    *   Byte 1: Battery Voltage -> Report via Serial (e.g., MQB `0x4D`). (`Raw * 0.05 + 5.0` V).

---

**CAN ID:** `0x1D0`

*   **Source:** Climate Control ECU
*   **Description:** Climate control status information (reported *by* the ECU).
*   **Period:** ~100ms
*   **CDCEmu Function (Receiver):** `processFrame` (Decodes Fan, Direction, AC, Recirc, Temps)
*   **Canbox Proxy Role (Receiver):** Use `peugeot_407_ms_1D0_climate_handler`.
*   **Signals (Based on `PSACANBridge`/`CDCEmu`):**
    *   Byte 5, Bits 2-0: Fan Speed (0-7)
    *   Byte 4: Air Direction Bits (Bit 6=Windshield, Bit 5=Face, Bit 4=Floor)
    *   Byte 3: Status Bits (Bit 7=Recirc, Bit 5=AC Active)
    *   Byte 1: Left Zone Temp (Raw, `(Raw*0.5)+14.0`? Needs mapping)
    *   Byte 0: Right Zone Temp (Raw, `(Raw*0.5)+14.0`? Needs mapping)
*   **Note:** This is the *status* message. The `simulator/clim` module sends *commands* on this ID with a different structure.

---

**CAN ID:** `0x1A1`
*   **Source:** BSI
*   **Description:** BSI Informational/Warning messages for display.
*   **Period:** Event-driven
*   **Signals:**
    *   Byte 1: Message Code (e.g., `0x8D`/`0xE8`=Low Tyre Pressure, `0xE5`=TPMS Fault).
    *   Byte 5, Bit 7: Show Flag (1 = Message Active).
*   **Proxy Use:** Monitor for TPMS codes (`0x8D`/`0xE8`/`0xE5`) to update `carstate.tpms_status` for mapping to MQB `0x46`.

---

**CAN ID:** `0x21F`

*   **Source:** Steering Wheel Controls Module
*   **Description:** Steering wheel button presses and scroll wheel activity.
*   **Period:** Event-driven
*   **CDCEmu Function (Receiver):** `processFrame` (Decodes Vol, Seek, Src, Scroll)
*   **Canbox Proxy Role (Receiver):** Use `peugeot_407_ms_21F_swc_handler`.
*   **Signals (Consistent):**
    *   Byte 3, Bit 3: Volume Up (1=Pressed)
    *   Byte 3, Bit 2: Volume Down (1=Pressed)
    *   Byte 7, Bit 7: Seek Forward / Next (1=Pressed)
    *   Byte 7, Bit 6: Seek Backward / Previous (1=Pressed)
    *   Byte 1, Bit 1: Source Button (?) (1=Pressed)
    *   Byte 0: Scroll Wheel (Value changes on scroll)

---

**CAN ID:** `0x221`
*   **Source:** BSI
*   **Description:** Instantaneous Trip Computer Data.
*   **Period:** ~500ms-1000ms
*   **Signals:**
    *   Byte 0: Contains display flags/stalk button info.
    *   Bytes 1-2: Instantaneous Fuel Consumption -> Report via Serial (RZC `0x33`, MQB `0x43`). (Scaling: L/100km * 10 for RZC).
    *   Bytes 3-4: Range (DTE) (km) -> Report via Serial (RZC `0x33`, MQB `0x43`).

---

**CAN ID:** `0x261` & `0x2A1`
*   **Source:** BSI
*   **Description:** Trip Computer Data for Trip 2 (`0x261`) & Trip 1 (`0x2A1`). **(Presence/Structure on 407 needs verification).**
*   **Period:** ~500ms-1000ms
*   **Signals (Assuming C4 B7 structure):**
    *   Byte 0: Average Speed (km/h) -> Report via Serial (RZC `0x34`/`0x35`, MQB `0x43`/`0x44`).
    *   Bytes 1-2: Distance Traveled (km) -> Report via Serial (RZC `0x34`/`0x35`, MQB `0x44`).
    *   Bytes 3-4: Average Fuel Consumption (L/100km * 10 for RZC) -> Report via Serial (RZC `0x34`/`0x35`, MQB `0x43`/`0x44`).

---

**CAN ID:** `0x2B6`, `0x336`, `0x3B6`
*   **Source:** BSI
*   **Description:** Vehicle Identification Number (VIN), split across messages.
*   **Period:** Infrequent / On Request?
*   **Signals (Consistent):** ASCII characters of VIN. -> Assemble for protocols needing VIN.

---

### Steering Wheel / Panel Controls

**CAN ID:** `0x21F`
*   **Source:** Steering Wheel Controls Module (COV)
*   **Description:** Steering wheel button presses and scroll wheel.
*   **Period:** Event-driven
*   **Proxy Role:** Detect state changes/deltas, map to serial protocol button codes (e.g., RZC `0x02`).
*   **Signals (Consistent):**
    *   Byte 3, Bit 3: Volume Up (RZC `0x14`, MQB `0x01`)
    *   Byte 3, Bit 2: Volume Down (RZC `0x15`, MQB `0x02`)
    *   Byte 7, Bit 7: Seek Forward / Next (RZC `0x12`, MQB `0x03`)
    *   Byte 7, Bit 6: Seek Backward / Previous (RZC `0x13`, MQB `0x04`)
    *   Byte 1, Bit 1: Source (?) (RZC `0x11`, MQB `0x0A`)
    *   Byte 0: Scroll Wheel Delta (Map to RZC `0x17`/`0x18`, MQB may use different codes/logic)

---

**CAN ID:** `0x3E5`
*   **Source:** Radio / Multimedia Panel (FMUX)
*   **Description:** Physical button presses on the main radio/media panel.
*   **Period:** Event-driven
*   **Proxy Role:** Optional: Detect state changes, map to serial protocol button codes.
*   **Signals (Bits map 1:1 to buttons, see `PSACAN.md`):** MENU, TEL, CLIM, TRIP, MODE, AUDIO, OK, ESC, DARK, Arrows, etc. Map to corresponding RZC `0x02` codes or MQB `0x20` codes if needed.

---

### Climate Control

**CAN ID:** `0x1D0`
*   **Source:** Climate Control ECU
*   **Description:** Climate control *status* reported by the ECU.
*   **Period:** ~100ms
*   **Proxy Role:** Read status, map to serial protocol AC message (RZC `0x21`?, MQB `0x21`).
*   **Signals (Based on `PSACANBridge`/`CDCEmu`):** See previous detailed description. Includes Fan Speed, Air Direction, AC/Recirc status, Temperatures (Raw).

---

### CD Changer Emulation Related Messages

**NOTE:** These IDs are primarily relevant if implementing CDC emulation. A basic proxy ignores/doesn't send these.

**A. Commands Sent by Radio -> Received by Emulator:**

*   **`0x131` (CDC Command):** Primary control message.
    *   Byte 7, Bit 7 (`E`): Enable/Disable Changer (Source Select)
    *   Byte 7, Bit 5 (`B`): Previous Track
    *   Byte 7, Bit 4 (`F`): Next Track
    *   Byte 6, Bit 7 (`X`): Fast Forward / Rewind
    *   Byte 6, Bit 5 (`I`): Track Intro Toggle
    *   Byte 6, Bit 4 (`P`): Play/Pause Toggle
    *   Byte 5, Bit 5 (`R`): Repeat Mode Toggle
    *   Byte 4, Bit 5 (`R`): Random Mode Toggle
    *   Bytes 1-2 (`DDDDDDDD`): Go to Disk N
    *   Byte 0 (`TTTTTTTT`): Go to Track N
    *   **Emulator Action:** Parse these bits and control the actual audio source.

**B. Status/Info Sent by Emulator -> Received by Radio/Display:**

*   **`0x0E2` (Yatour? Current Disk Number):** Byte 7, Bits 3-0 = Active Disk # (1-15).
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

**C. Related Status (Monitored by Emulator):**

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

### 3. Relevance to `canbox-renode` RZC Proxy:

*   **CDC Emulation is Separate:** The primary goal of the RZC proxy is to relay *vehicle status* to the aftermarket HU, not to emulate a CDC for the *factory* HU.
*   **Ignore CDC Traffic:** The `canbox-renode` firmware, when acting as an RZC proxy, should **ignore** incoming CDC commands (`0x131` command bits) and **not send** any CDC status messages (`0x0E2`, `0x162`, etc.).
*   **Future Possibility:** If a user *keeps* the factory radio and wants to integrate the Android HU's audio *via* the CDC input, the `canbox` firmware *could* be extended to include CDC emulation logic, translating between the Android HU's media controls (via RZC or another protocol) and the necessary PSA CAN CDC messages. This is a significant development effort.

