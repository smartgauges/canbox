# PSA CAN Bus Data Analysis

This document details CAN bus messages from PSA (Peugeot Société Anonyme) vehicles, specifically focusing on data from Peugeot 407 '2006, Citroën C4 '2012 (B7), and Citroën C4 '2008 (B3/B5) models. 

## Legend

* **Berlingo** - Based on Berlingo '2011
* **C4 (B7)** - Based on Citroën C4 '2012 (B7)
* **C4 (B3/B5)** - Based on Citroën C4 '2008 (B3/B5)
* **C4 Sedan** - Based on Citroën C4 Sedan
* **407** - Based on Peugeot 407 Coupé '2006
* **Yatour** - Based on Yatour YT-M06
* **308** - Based on BSI from Peugeot 308

## Color Coding for Sources

* <span style="background: green; color: white; padding: 2px 4px;">RT6</span> -  Messages from the RT6 telematics system.
* <span style="background: yellow; padding: 2px 4px;">BSI</span> - Messages from the Body Systems Interface.
* <span style="background: blue; color: white; padding: 2px 4px;">Steering wheel</span> - Messages related to steering wheel controls.
* <span style="background: red; padding: 2px 4px;">Display</span> - Messages related to display information.
* <span style="background: BlueViolet; color: white; padding: 2px 4px;">Parktronic</span> - Messages from the parktronic system.
* <span style="background: Chocolate; padding: 2px 4px;">Yatour</span> - Messages from the Yatour digital music changer.
* <span style="background: DarkCyan; padding: 2px 4px;">Climate</span> - Messages related to the climate control system.
* <span style="background: GreenYellow; padding: 2px 4px;">FMUX</span> - Messages from the Front Multiplexing Unit.
* <span style="background: HotPink; padding: 2px 4px;">Radio</span> - Messages originating from the radio unit.
* <span style="background: LightCoral; padding: 2px 4px;">Dashboard</span> - Messages intended for the instrument cluster display.

## ID Index

(Table summarizing CAN ID ranges and their likely sources/functions - *omitted for brevity, see HTML version for full table*)

## CAN Message Details

### ID: 036 
**BSI Ignition, Dashboard lightning**

* **Network:** CAN-INFO
* **Source:** BSI
* **Destination:** Display, Radio
* **Description:**  Indicates ignition mode and dashboard lighting status.
* **Period:** 100ms

| Bit Offset | Bits      | Description                                    |
|------------|-----------|------------------------------------------------|
| 7          | 00001110  | Economy mode enabled                          |
| 6          | 0000000   | Dashboard lightning enabled                     |
| 5          | 0000000   | Disables dark mode, turns off climate display  |
| 4          | BBBB      | Brightness for dashboard lightning              |
| 0-2        | MMM       | Ignition mode (On, Off, Unknown)               |

### ID: 0A4 
**Current CD Track, Radio Text (ISO 15765-2)**

* **Network:** CAN-INFO
* **Source:** Radio
* **Destination:** Display
* **Description:** Transmits current CD track information or radio text using ISO 15765-2 protocol.
* **Period:** Not periodic

| Bit Offset | Bits      | Description                                         |
|------------|-----------|-----------------------------------------------------|
| 3          | PPPP      | Page number (Radio text: Page 1, CD track: Page 2) |
| 2          | A         | Message contains track author data                 |
| 1          | NNNNNNNN  | Track number                                       |
| N/A        | TEXT_DATA | Track author and name (ASCII encoded)              |

### ID: 0B6 
**BSI RPM/Speed**

* **Network:** CAN-INFO
* **Source:** BSI
* **Destination:**  
* **Description:**  Vehicle speed, RPM, odometer, and fuel consumption data.
* **Period:** 50ms

| Bit Offset | Bits           | Description                                     |
|------------|----------------|-------------------------------------------------|
| 5-6        | MMMMMMMMMMMMM  | Tachometer (RPM)                                |
| 3-4        | SSSSSSSSSSSSSSSS | Speed * 100 (km/h)                              |
| 1-2        | TTTTTTTTTTTTTTTT | Odometer (cm)                                   |
| 0          | FFFFFFFF       | Fuel consumption counter                         |

### ID: 0DF 
**Display Menu**

* **Network:** CAN-INFO
* **Source:** Radio or Display
* **Destination:** Display
* **Description:** Controls display menu behavior, including main menu, popups, and dark mode.
* **Period:** 500ms

| Bit Offset | Bits | Description                                                                                                   |
|------------|------|---------------------------------------------------------------------------------------------------------------|
| 7          | M    | Main menu, ok-menu, or Dark button (short press)                                                              |
| 6          | U    | Unknown, non-constant value                                                                                   |
| 5          | UUU  | Unknown, non-constant value                                                                                   |
| 4          | UUU  | Unknown, non-constant value                                                                                   |
| 0          | Z    | Popup status (menu, track list, PTY list, audio menu), dark mode, or inverse mode enabled (short press)       |

### ID: 0E1 
**Parktronic Information**

* **Network:** CAN-INFO
* **Source:** Parktronic
* **Destination:** Display, Radio
* **Description:**  Parktronic sensor data, beep periods, and display control.
* **Period:** 100ms

| Bit Offset | Bits      | Description                                  |
|------------|-----------|----------------------------------------------|
| 7          | XXXXXXXX  | Initial value (ignition state dependent)     |
| 6          | R         | Right channel sound enabled                 |
| 5          | L         | Left channel sound enabled                  |
| 4          | F         | Front (1) or rear (0) channels              |
| 3          | S         | Sound enabled                               |
| 1-2        | HHHHHH    | Beep period (000000 = continuous beep)      |
| 0          | III       | Rear left sensor distance                   |
| 7          | KKKKK     | Rear center sensor distance                 |
| 0          | LLL       | Rear right sensor distance                  |
| 7          | MMMMM     | Front left sensor distance                  |
| 0          | QQQ       | Front center sensor distance                |
| 7          | RRR       | Front right sensor distance                 |
| 2          | P         | Show parktronic window                      |
| 0          | XXXXXXXX  | Initial value (ignition state dependent)     |

**(Tables detailing sensor distance values - omitted for brevity, see HTML version for full tables)**

### ID: 0E2 
**Yatour Current Disk Number**

* **Network:** CAN-INFO
* **Source:** Yatour
* **Description:**  Currently selected disk number in Yatour CD changer emulator.
* **ID:** 0E2

| Bit Offset | Bits | Description                                      |
|------------|------|--------------------------------------------------|
| 7          | DDDD | Current disk number (up to 15)                   |
| N/A        |      | Unknown, possibly related to CD changer status |

### ID: 0E6 (C4(B7))
**Wheels Rotation, Voltage (C4(B7) Specific)**

* **Network:** CAN-INFO
* **Source:** BSI
* **Description:** Wheel rotation counters from ABS and battery voltage.
* **ID:** 0E6 (C4(B7))

| Bit Offset | Bits           | Description                                  |
|------------|----------------|----------------------------------------------|
| 5-6        | LLLLLLLLLLLLLL | Left wheel rotation counter (ABS impulses)     |
| 3-4        | RRRRRRRRRRRRRR | Right wheel rotation counter (ABS impulses)    |
| 0          | VVVVVVVV       | Voltage (calculated from raw value)           |

### ID: 0F6 
**BSI - Turn Indicators, Temperature, Odometer**

* **Network:** CAN-INFO, CAN-CAR
* **Source:** BSI
* **Destination:** Display, Dashboard
* **Description:**  Turn indicator status, coolant temperature, odometer, and potentially fuel consumption counter.
* **ID:** 0F6

| Bit Offset | Bits        | Description                               |
|------------|-------------|-------------------------------------------|
| 7          | 1           | Ignition status (1-on, 0-off)            |
| 6          | 110         | -                                         |
| 5          | CCCCCCCCCC  | Coolant Temperature (°C)                   |
| 4          | ZZZZZZZZZZZZZZZZ | Odometer (cm)                            |
| 3          | TTTTTTTT    | Ambient Temperature (°C)                   |
| 2          | R           | Reverse gear light                       |
| 1          | Z           | Right turn indicator light               |
| 0          | F           | Left turn indicator light                 |

### ID: 122 
**Universal Multiplexed Panel (Multimedia Control)**

* **Network:** CAN-INFO
* **Source:** Universal Panel (FMUX)
* **Destination:** Radio
* **Description:**  Data from a universal multiplexed panel, likely for multimedia controls. 
* **ID:** 122

| Bit Offset | Bits | Description                                  |
|------------|------|----------------------------------------------|
| 7          | A    | Button "1"                                  |
| 7          | B    | Button "2"                                  |
| 7          | C    | Button "3"                                  |
| 7          | D    | Button "4"                                  |
| 7          | E    | Button "5"                                  |
| 7          | F    | Button "6"                                  |
| 7          | G    | Button "EJECT"                                |
| 7          | H    | Button "ESC"                                  |
| 6          | I    | Button "PAGE SELECTION"                       |
| 6          | J    | Button "TA INFO"                              |
| 6          | K    | Button "SOURCE"                               |
| 6          | L    | Button "MENU"                                 |
| 6          | M    | Button "LIST"                                 |
| 6          | N    | Button "EQUALIZER"                            |
| 6          | O    | Button "BAND"                                 |
| 6          | P    | Button "LEFT"                                 |
| 5          | Q    | Button "RIGHT"                                |
| 5          | R    | Button "UP"                                   |
| 5          | S    | Button "DOWN"                                 |
| 5          | T    | Button "OK"                                   |
| 1-2        | XXXXXXXX | Left control rotation (CW increase)         |
| 0          | YYYYYYYY | Right control rotation (CW increase)        |

### ID: 125 
**CD Track List, Radio List (ISO 15765-2)**

* **Network:** CAN-INFO
* **Source:** Radio
* **Destination:** Display
* **Description:**  Transmits CD track lists or radio station lists using ISO 15765-2 protocol.
* **ID:** 125

| Bit Offset | Bits        | Description                                  |
|------------|-------------|----------------------------------------------|
| 7          | PPPP        | Page number (Radio bands, CD tracklist)      |
| 6          | TTTTTTTT    | Track count                                  |
| 5          | AAAAAAAA    | Current view offset                          |
| 5          | S           | "Show track list" flag                       |
| 5          | E           | "List moved" flag                            |
| 4          | BBBB        | Current track offset in list                 |
| 3          | MMMMMMMM    | Text data descriptor                         |
| N/A        | JJJJJJJJ x20 | Track author name (ASCII)                     |
| N/A        | HHHHHHHH x20 | Track name (ASCII)                          |

### ID: 128 
**Dashboard Lights Status**

* **Network:** CAN-INFO
* **Source:** BSI
* **Destination:** Dashboard
* **Description:**  Indicates status of various dashboard lights (indicators, warnings).
* **ID:** 128

| Bit Offset | Bits | Description                                  |
|------------|------|----------------------------------------------|
| 7          | S    | Driver seat belt warning light               |
| 7          | P    | Parking brake light                          |
| 6          | D    | Door/trunk open indicator                    |
| 5          | G    | Sidelights on indicator                      |
| 5          | F    | Low beam on indicator                        |
| 5          | E    | High beam on indicator                       |
| 5          | D    | Front fog lights on indicator                |
| 5          | C    | Rear fog light on indicator                 |
| 5          | B    | Right turn indicator on                      |
| 5          | A    | Left turn indicator on                       |
| 4          | 1    | Low fuel level warning                       |
| 3          | 1    | Seat belts warning blinking                  |
| 3          | 0    | Seat belts warning                           |
| 2          | 1    | Seat belts warning blinking                  |
| 2          | 0    | Seat belts warning                           |
| 0-1        | BBBB | Dashboard backlighting brightness            |

### ID: 131 
**CD Changer Command**

* **Network:** CAN-INFO
* **Source:** Radio
* **Description:**  Commands for controlling a CD changer (or emulator).
* **ID:** 131

| Bit Offset | Bits     | Description                          |
|------------|----------|--------------------------------------|
| 7          | E        | Enable/disable CD changer             |
| 7          | B        | Single track back                     |
| 7          | F        | Single track forward                  |
| 6          | X        | Fast forward/backward, track intro      |
| 6          | S        | Go to track start                      |
| 6          | I        | Track intro mode                       |
| 6          | P        | Enable/disable playing                 |
| 5          | R        | Repeat mode                            |
| 4          | R        | Random mode                            |
| 1-2        | DDDDDDDD | Go to disk N                          |
| 0          | TTTTTTTT | Go to track N                         |

### ID: 162 
**Yatour Current Disk Status**

* **Network:** CAN-INFO
* **Source:** Yatour
* **Description:**  Status information about the currently selected disk in the Yatour emulator.
* **ID:** 162

| Bit Offset | Bits   | Description                                      |
|------------|--------|--------------------------------------------------|
| 7          | E      | Radio power status (0 = off)                   |
| 6          | L      | Disk change in progress ("Please wait" display) |
| 2-4        | SSS    | Status icon (Play, Pause, Stop, etc.)          |
| 1          | DDDD   | Current disk number                             |
| 0          | 000110 | CD changer source identifier                  |

### ID: 165 
**Radio Status**

* **Network:** CAN-INFO
* **Source:** Radio
* **Description:**  Overall radio status, including band, source, and CD changer availability.
* **ID:** 165

| Bit Offset | Bits | Description                               |
|------------|------|-------------------------------------------|
| 7          | P    | Radio enabled flag                        |
| 6          | M    | Mute status                               |
| 5          | DD   | CD disk availability status                |
| 4          | C    | CD changer available flag                 |
| 2-4        | SSS  | Current audio source (Tuner, CD, AUX, etc.) |

### ID: 167 
**Display Status**

* **Network:** CAN-INFO
* **Source:** Display
* **Destination:** 
* **Description:**  Indicates if the display is on or off (likely due to "Dark" mode).
* **ID:** 167

| Bit Offset | Bits | Description                         |
|------------|------|-------------------------------------|
| 6          | D    | Display power state (0 = on, 1 = off) |

### ID: 1A0 
**Yatour CD Changer Status Flags**

* **Network:** CAN-INFO
* **Source:** Yatour
* **Description:**  Status flags for Yatour CD changer emulation modes (Intro, Random, Repeat).
* **ID:** 1A0

| Bit Offset | Bits | Description             |
|------------|------|-------------------------|
| 5          | I    | Intro mode              |
| 4          | R    | Random mode             |
| 3          | R    | Repeat mode             |

### ID: 1A1 
**Informational Message (BSI to Display)**

* **Network:** CAN-INFO
* **Source:** BSI
* **Destination:** Display
* **Description:**  General purpose informational messages from BSI to display, including warning and status messages.
* **ID:** 1A1

| Bit Offset | Bits      | Description                                  |
|------------|-----------|----------------------------------------------|
| 7          | XXXXXXXX  | Window control (show/hide/clear)             |
| 6          | YYYYYYYY  | Message code (identifies message type)        |
| 5          | Z         | "Show" flag                                  |
| N/A        |           | (Padding bytes - likely message parameters)   |

**(Table listing message codes and their English/Russian descriptions - omitted for brevity, see HTML version for full table)**

### ID: 1A2 
**Yatour Tracks Count**

* **Network:** CAN-INFO
* **Source:** Yatour
* **Description:**  Total number of tracks on the current disk in the Yatour emulator.
* **ID:** 1A2

| Bit Offset | Bits       | Description            |
|------------|------------|------------------------|
| 7          | CCCCCCCC   | Tracks count           |

### ID: 1A5 
**Radio Sound Level**

* **Network:** CAN-INFO
* **Source:** Radio
* **Destination:** Display
* **Description:**  Current volume level and volume menu display control. 
* **ID:** 1A5

| Bit Offset | Bits  | Description                        |
|------------|-------|------------------------------------|
| 7          | BBB   | Volume scroll status (000=scrolling) |
| 6          | VVVVV | Sound volume (0-30)                 |

### ID: 1D0 
**Climate Control Information**

* **Network:** CAN-INFO
* **Source:** Climate Control
* **Description:**  Climate control settings such as fan speed, air direction, temperature, and recirculation.
* **ID:** 1D0

| Bit Offset | Bits  | Description                             |
|------------|-------|-----------------------------------------|
| 5          | FFF   | Fan speed (0-7)                         |
| 4          | DDD   | Air direction (Front, Up, Down, etc.)    |
| 3          | A     | Air recirculation enabled                |
| 3          | W     | Windshield blowing enabled              |
| 1          | LLLLL | Left zone temperature                   |
| 0          | RRRRR | Right zone temperature                  |

**(Table detailing temperature values - omitted for brevity, see HTML version for full table)**

### ID: 1E0 
**Radio Control Flags**

* **Network:** CAN-INFO
* **Source:** Radio
* **Destination:** Display
* **Description:**  Flags for controlling radio functions like intro, random, repeat, RDS, and radio text.
* **ID:** 1E0

| Bit Offset | Bits | Description                                |
|------------|------|--------------------------------------------|
| 7          | I    | Activate track intro                       |
| 5          | R    | Activate random play                       |
| 4          | R    | CD repeat mode                             |
| 3          | A    | Activate alternative frequencies (RDS)       |
| 2          | G    | REG mode activated                         |
| 0          | T    | Activate radio text                          |

### ID: 1E2 
**Yatour Current Track Time**

* **Network:** CAN-INFO
* **Source:** Yatour
* **Description:**  Current track playback time from the Yatour emulator.
* **ID:** 1E2

| Bit Offset | Bits       | Description                            |
|------------|------------|----------------------------------------|
| 7          | TTTTTTTT   | Current track number                    |
| 5-6        |            | Track switching status (FF FF = switching) |
| 4          | M|MMMMMMMM   | Current track minute (FF = "--" during switching) |
| 3          | SSSSSSS    | Current track second (7F = "--" during switching) |
| 0          |            | Track switching status (80 = switching)            |

### ID: 1E5 
**Audio Settings**

* **Network:** CAN-INFO
* **Source:** Radio
* **Destination:** Display
* **Description:**  Audio settings like balance, fader, bass, treble, loudness, and ambiance.
* **ID:** 1E5
* **Period:** 500ms

| Bit Offset | Bits  | Description                    |
|------------|-------|--------------------------------|
| 7          | S     | Show balance menu (Left/Right)   |
| 6          | sssssss | Balance (Left/Right)            |
| 5          | F     | Show balance menu (Rear/Front)   |
| 4          | fffffff | Balance (Rear/Front)            |
| 3          | B     | Show bass menu                   |
| 2          | bbbbbbb | Bass                             |
| 1          | T     | Show treble menu                 |
| 0          | ttttttt | Treble                           |
| 7          | L     | Show loudness correction menu     |
| 7          | l     | Loudness correction value        |
| 7          | A     | Show automatic volume menu       |
| 7          | aaa   | Automatic volume value           |
| 7          | P     | Show musical ambiance menu       |
| 7          | ppppp | Musical ambiance value           |

**(Tables detailing Bass/Treble/Balances and Musical ambiance values - omitted for brevity, see HTML version for full tables)**

### ID: 1ED 
**Display Conditioning Commands**

* **Network:** CAN-INFO
* **Source:** Display
* **Destination:** 
* **Description:**  Commands to control display conditioning, like simultaneous LH-RH control or A/C deactivation.
* **ID:** 1ED

| Bit Offset | Bits      | Description                                         |
|------------|-----------|-----------------------------------------------------|
| 0          | XXXXXXXX  | Conditioning command value (Default, LH-RH control, A/C OFF) |

### ID: 21F 
**Steering Wheel Radio Controls**

* **Network:** CAN-INFO
* **Source:** Steering Wheel Controls
* **Destination:** Radio
* **Description:**  Steering wheel button presses for radio control. 1 = button pressed, 0 = button released. Band and Source buttons are not reflected in this message.
* **ID:** 21F

| Bit Offset | Bits     | Description                |
|------------|----------|----------------------------|
| 7          | F        | Forward button             |
| 6          | B        | Backward button            |
| 5          | X        | Unknown, non-constant value |
| 3          | U        | Volume Up button           |
| 2          | D        | Volume Down button         |
| 1          | S        | Source button              |
| 0          | RRRRRRRR | Scroll wheel value          |

### ID: 220 
**Door Status**

* **Network:** CAN-INFO
* **Source:** BSI
* **Destination:**
* **Description:**  Indicates door open/closed status. 1 = open, 0 = closed.
* **ID:** 220

| Bit Offset | Bits | Description        |
|------------|------|--------------------|
| 7          | X    | Door Front Left    |
| 6          | X    | Door Front Right   |
| 5          | X    | Door Back Left     |
| 4          | X    | Door Back Right    |
| 3          | X    | Door Trunk         |

### ID: 221 
**Trip Computer Information**

* **Network:** CAN-INFO
* **Source:** BSI
* **Destination:** Display
* **Description:** Trip computer data including fuel consumption and range.
* **ID:** 221

| Bit Offset | Bits           | Description                                      |
|------------|----------------|--------------------------------------------------|
| 7          | l              | Liters per 100 km is "--.-" flag               |
| 6          | r              | Rest of run is "----" flag                       |
| 4          | T              | Trip mode switch down flag                       |
| 2          | X              | Unused button light lever / voice command button |
| 4-5        | LLLLLLLLLLLLLL | Liters per 100km value                           |
| 2-3        | RRRRRRRRRRRRRR | Rest of run on current fuel level                 |
| 0-1        | FFFFFFFFFFFFFFFF| Rest of run to empty tank                        |

### ID: 225 (C4(B3/B5))
**Radio Frequency (C4(B3/B5) Specific)**

* **Network:** CAN-INFO
* **Source:** Radio
* **Destination:** Display
* **Description:** Radio frequency and band information for older Citroen C4 models.
* **ID:** 225

| Bit Offset | Bits        | Description                               |
|------------|-------------|-------------------------------------------|
| 7          | L           | Station list open flag                    |
| 6          | D           | Frequency tuning direction (Up/Down)      |
| 5          | A           | Autoscan active flag                      |
| 4          | R           | RDS search active flag                    |
| 3          | T           | Frequency tuning active flag                |
| 2          | P           | PTY search active flag                    |
| 1-3        | SSS         | Band memory position (1-6, 7=mem--, 0=none)  |
| 0-2        | XXX         | Radio band (FM1, FM2, FMAST, AM, ---)      |
| 0-1        | FFFFFFFFFFFF| Radio frequency (calculated from raw value)  |

### ID: 225 (C4(B7))
**Radio Frequency and Signal Level (C4(B7) Specific)**

* **Network:** CAN-INFO
* **Source:** Radio
* **Destination:** Display
* **Description:** Radio frequency, band, and signal level information for newer Citroen C4 (B7) models.
* **ID:** 225

| Bit Offset | Bits        | Description                               |
|------------|-------------|-------------------------------------------|
| 3-4        | SSS         | Band memory position (1-6, 0=none)       |
| 2-4        | XXX         | Radio band (FM1, FM2, DAB, AM, LongWave) |
| 0-1        | FFFFFFFFFFFF| Radio frequency (calculated from raw value)  |
| 0          | LLLLLLLL    | Radio signal level (0-100)                |

### ID: 261 (C4(B7))
**Trip Computer Data 2 (C4(B7) Specific)**

* **Network:** CAN-INFO
* **Source:** BSI
* **Destination:** Display
* **Description:** Trip computer data - average speed and mileage since reset for newer Citroen C4 (B7) models.
* **ID:** 261 (C4(B7))

| Bit Offset | Bits           | Description                             |
|------------|----------------|-----------------------------------------|
| 7          | XXXXXXXX       | Average speed (0-250 km/h)              |
| 5-6        | YYYYYYYYYYYYYY | Vehicle mileage after reset              |
| 0-1        | ZZZZZZZZZZZZZZ | Fuel consumption (Liters per 100km)      |

### ID: 265 
**RDS Data**

* **Network:** CAN-INFO
* **Source:** Radio
* **Destination:** Display
* **Description:** RDS (Radio Data System) information, including station name, PTY, and TA flags.
* **ID:** 265

| Bit Offset | Bits      | Description                       |
|------------|-----------|-----------------------------------|
| 7          | S         | RDS activated flag                |
| 6          | F         | RDS found flag                    |
| 5          | T         | TA (Traffic Announcement) activated |
| 4          | F         | TA found flag                     |
| 3          | R         | Traffic info flag                 |
| 2          | R         | REG mode activated                |
| 1          | P         | PTY flag                          |
| 1          | M         | PTY menu shown flag               |
| 0          | XX        | PTY info (hide/small/big)         |
| 0          | PPPPPPPP  | PTY menu selected item            |
| 0          | CCCCCCCC  | Current station PTY               |

**(Table detailing PTY menu items - omitted for brevity, see HTML version for full table)**

### ID: 276 (C4(B7))
**Date and Time (C4(B7) Specific)**

* **Network:** CAN-INFO
* **Source:** BSI
* **Destination:** Display
* **Description:**  Date and time information for newer Citroen C4 (B7) models.
* **ID:** 276

| Bit Offset | Bits      | Description              |
|------------|-----------|--------------------------|
| 7          | A         | Time format (12h/24h)    |
| 6          | YYYYYYY   | Year (2000-2125)          |
| 5          | R         | Date/Time reset flag     |
| 4          | MMMM      | Month (1-12)             |
| 3          | DDDDD     | Day (1-31)               |
| 2          | HHHHH     | Hour (0-23)              |
| 1          | ZZZZZZ    | Minutes (0-59)           |

### ID: 2A1 (C4(B7))
**Trip Computer Data 1 (C4(B7) Specific)**

* **Network:** CAN-INFO
* **Source:** BSI
* **Destination:** Display
* **Description:** Trip computer data - average speed and mileage since reset for newer Citroen C4 (B7) models.
* **ID:** 2A1 (C4(B7))

| Bit Offset | Bits           | Description                             |
|------------|----------------|-----------------------------------------|
| 7          | XXXXXXXX       | Average speed (0-250 km/h)              |
| 5-6        | YYYYYYYYYYYYYY | Vehicle mileage after reset              |
| 0-1        | ZZZZZZZZZZZZZZ | Fuel consumption (Liters per 100km)      |

### ID: 2B6 
**VIN (Last 8 Digits)**

* **Network:** CAN-INFO
* **Source:** BSI
* **Destination:** Radio
* **Description:** Last 8 digits of the Vehicle Identification Number (VIN), ASCII encoded.
* **ID:** 2B6

| Bit Offset | Bits           | Description                      |
|------------|----------------|----------------------------------|
| 0-7        | VVVVVVVVVVVVVVVV | Last 8 VIN digits (ASCII encoded) |

### ID: 325 
**CD Tray Information**

* **Network:** CAN-INFO
* **Source:** Radio
* **Description:** CD tray status - disk presence, reading status, and inserting/ejecting status.
* **ID:** 325

| Bit Offset | Bits | Description                       |
|------------|------|-----------------------------------|
| 6          | I    | Disk inserting/ejecting in progress |
| 5          | R    | Disk ready for reading             |
| 4          | P    | Disk present                       |

### ID: 30D 
**Wheel Rotation Speed (CAN-IS Network)**

* **Network:** CAN-IS (10ms period)
* **Source:**  
* **Destination:** 
* **Description:**  Individual wheel rotation speeds from ABS sensors. High-speed CAN data.
* **ID:** 30D

| Bit Offset | Bits           | Description           |
|------------|----------------|-----------------------|
| 5-6        | FLFLFLFLFLFLFLFL | Front-left wheel speed  |
| 3-4        | FRFRFRFRFRFRFRFR | Front-right wheel speed |
| 1-2        | RLRLRLRLRLRLRLRL | Rear-left wheel speed   |
| 0          | RRRRRRRRRRRRRRRR | Rear-right wheel speed  |

### ID: 336 
**VIN (First 3 Letters)**

* **Network:** CAN-INFO
* **Source:** BSI
* **Description:** First 3 letters of the Vehicle Identification Number (VIN), ASCII encoded.
* **ID:** 336

| Bit Offset | Bits           | Description                      |
|------------|----------------|----------------------------------|
| 0-7        | VVVVVVVVVVVVVVVV | First 3 VIN letters (ASCII encoded) |

### ID: 365 
**CD Disk Information**

* **Network:** CAN-INFO
* **Source:** Radio
* **Destination:** Display
* **Description:** CD disk information like track count, total time, and CD type.
* **ID:** 365

| Bit Offset | Bits       | Description                     |
|------------|------------|---------------------------------|
| 7          | NNNNNNNN   | Number of tracks on CD           |
| 5-6        | M|MMMMMMMM   | Disk full time (minutes)        |
| 3-4        | SSSSSSSS   | Disk full time (seconds)        |
| 2          | mmmmmmmm   | Current track minute             |
| 1          | ssssssss   | Current track second             |
| 0          | M          | CD type (1 = MP3, 0 = Audio CD) |

### ID: 39B 
**Set System Time**

* **Network:** CAN-INFO
* **Source:** Display/RT6
* **Destination:** BSI
* **Description:** Command to set the system date and time, likely sent from the head unit to the BSI.
* **ID:** 39B

| Bit Offset | Bits      | Description              |
|------------|-----------|--------------------------|
| 7          | A         | Time format (12h/24h)    |
| 6          | YYYYYYY   | Year (2000-2125)          |
| 4          | MMMM      | Month (1-12)             |
| 3          | DDDDD     | Day (1-31)               |
| 2          | HHHHH     | Hour (0-23)              |
| 1          | ZZZZZZ    | Minutes (0-59)           |

### ID: 3A5 
**Current CD Track Info**

* **Network:** CAN-INFO
* **Source:** Radio
* **Destination:** Display
* **Description:** Information about the currently playing CD track, including track number and time elapsed.
* **ID:** 3A5

| Bit Offset | Bits       | Description                     |
|------------|------------|---------------------------------|
| 7          | NNNNNNNN   | Track number                    |
| 6          | M|MMMMMMMM   | Total minutes (FF = "--" if unavailable)        |
| 5          | SSSSSSSS   | Total seconds (FF = "--" if unavailable)        |
| 4          | mmmmmmmm   | Current minutes (FF = "--" if unavailable)      |
| 3          | ssssssss   | Current seconds (7F = "--" if unavailable)      |

### ID: 3B6 
**VIN (Characters 4-9)**

* **Network:** CAN-INFO
* **Source:** BSI
* **Description:** Characters 4-9 of the Vehicle Identification Number (VIN), ASCII encoded.
* **ID:** 3B6

| Bit Offset | Bits           | Description                      |
|------------|----------------|----------------------------------|
| 0-7        | VVVVVVVVVVVVVVVV | VIN characters 4-9 (ASCII encoded) |

### ID: 3E5 
**Radio Button Presses**

* **Network:** CAN-INFO
* **Source:** Radio
* **Destination:** Display
* **Description:**  Indicates which buttons on the radio/multimedia panel are being pressed.  "Band" and "Source" buttons are not reflected in this message. Menu, Clim & Trip switches will directly display to those modes on the head unit.
* **ID:** 3E5

| Bit Offset | Bits | Description                  |
|------------|------|------------------------------|
| 7          | L    | Menu button                  |
| 7          | T    | Tel (Telephone/Phone) button |
| 6          | J    | Clim (Climate) button        |
| 6          | H    | Trip button                  |
| 6          | M    | Mode button                  |
| 6          | G    | Audio button                 |
| 5          | Q    | OK button                    |
| 5          | E    | Esc (Escape) button          |
| 5          | K    | Dark button                  |
| 4          | R    | Up button                    |
| 4          | S    | Down button                  |
| 4          | P    | Right button                 |
| 4          | N    | Left button                  |

### ID: 3F6 
**Date and Time (Possible)**

* **Network:** CAN-INFO
* **Source:** Radio
* **Destination:** Display
* **Description:**  Potentially date and time information, though the exact format and purpose are unclear.
* **ID:** 3F6

| Bit Offset | Bits            | Description                                 |
|------------|-----------------|---------------------------------------------|
| 0-2        | XXXXXXXXXXXXXXXX| Time in seconds (suspected)                  |
| 5          | Z               | Time format (0 - 12h, 1 - 24h)            |

### ID: 51D 
**Parktronic Status (Example Data)**

* **Network:** CAN-INFO
* **Source:** Parktronic
* **Description:** Example data observed for parktronic system.  Further decoding needed.
* **ID:** 51D

| Bit Offset | Bits | Description                                                                                                                               |
|------------|------|-----------------------------------------------------------------------------------------------------------------------------------------|
| All        |      | Example Data: `01 00 00 00 00 00 00 00` (no sensors, on live car), `05 00 53 00 01 00 00 00` (1 sensor), `21 00 21 00 05 00 00 00` (3 sensors, hot plug) etc. |

### ID: 5E0 
**Radio Hardware/Software Info**

* **Network:** CAN-INFO
* **Source:** Radio
* **Description:**  Likely hardware and software version information for the radio unit itself.
* **ID:** 5E0

| Bit Offset | Bits           | Description             |
|------------|----------------|-------------------------|
| 3-4        | VVVVVVVVvvvvvvvv | Firmware version (VV.vv) |

### ID: 5E5 
**Display Hardware/Software Info**

* **Network:** CAN-INFO
* **Source:** Display
* **Description:**  Likely hardware and software version information for the display unit.
* **ID:** 5E5

| Bit Offset | Bits           | Description             |
|------------|----------------|-------------------------|
| 3-4        | VVVVVVVVvvvvvvvv | Firmware version (VV.vv) |

This document is a work in progress, and further analysis may be needed to fully understand all the messages and their parameters. Contributions and corrections are welcome.
