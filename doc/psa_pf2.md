# CAN Box Implementation Guide for PSA PF2 Platform (e.g., Peugeot 407)

### 1. Introduction

This guide details the specifics of adapting the `canbox` firmware project to vehicles based on the **PSA PF2 platform**. This platform was used for compact and mid-size Peugeot and Citroën vehicles roughly from the early 2000s to the mid-2010s.

**Key Characteristics of PF2 Relevant to CAN Box:**

*   **CAN-Centric Architecture:** PF2 vehicles heavily utilize CAN bus networks for communication between Electronic Control Units (ECUs).
*   **Multiple Buses:** Typically includes several CAN buses (Comfort, Body, Engine/Powertrain, Diagnostics).
*   **BSI (Built-in Systems Interface):** Acts as a central gateway and controller for many functions, especially on the Comfort and Body buses.
*   **Target Vehicle:** Your **Peugeot 407 (2008MY)** is a prime example of a PF2 platform vehicle.

The goal is to configure the `canbox` firmware to correctly listen to the appropriate CAN bus, decode relevant messages, and generate the necessary signals and serial protocol data for an aftermarket Android head unit.

### 2. Target CAN Network: CAN Comfort (CAN-INFO)

For interfacing with head unit functions, the primary network of interest on the PF2 platform is typically the **CAN Comfort** bus (sometimes referred to as CAN-INFO or Body CAN).

*   **Typical Speed:** **125 kbps** (as confirmed for your Peugeot 407).
*   **Relevant Signals:** This bus usually carries information crucial for head unit integration:
    *   Ignition Status (ACC/IGN)
    *   Door Open/Closed Status (All doors, Bonnet, Tailgate)
    *   Lighting Status (Parking lights, Headlights, Illumination dimming)
    *   Steering Wheel Controls
    *   Climate Control Status
    *   Parking Sensor Data
    *   Reverse Gear Status
    *   Vehicle Speed (sometimes duplicated from Engine CAN)
    *   Outside Temperature
    *   VIN Information
    *   Trip Computer Data

*   **Connection:** You need to connect the `canbox` hardware's CAN H and CAN L lines to the CAN Comfort H and CAN L wires in the vehicle's wiring harness (or via an OBD2 breakout if the Comfort bus is accessible there, which is common).

### 3. Core PF2 CAN Messages for `canbox` Functionality

Based on the provided `PSACAN.md`, `peugeot407can.yaml`, and general PF2 knowledge, here are the likely essential CAN message IDs to target in `cars/peugeot_407.c`.

**Disclaimer:** **These IDs and data formats MUST BE VERIFIED on your specific 2008 Peugeot 407 using CAN sniffing tools (SavvyCAN, `cansniffer`).** Variations exist even within the same model year or trim level.

*   **Ignition/Accessory Status:**
    *   **Likely ID:** `0x036` (Source: BSI) - Contains ignition mode and dashboard lighting.
    *   **Likely ID:** `0x0B6` (Source: BSI) - May also contain ignition/engine running status.
    *   **`canbox` Need:** Decode bits indicating Key Presence, ACC, IGN ON, and potentially Engine Running states to control the `ACC` output signal and internal state (`carstate.acc`, `carstate.ign`).
*   **Illumination/Lights:**
    *   **Likely ID:** `0x036` (Source: BSI) - Contains dashboard brightness/lighting enabled flags.
    *   **Likely ID:** `0x128` (Source: BSI) - Contains status for side lights, low beams, high beams.
    *   **`canbox` Need:** Decode dashboard lighting status/brightness to control the `ILLUM` output signal. Decode headlight status for internal state (`carstate.park_lights`, `carstate.near_lights`).
*   **Reverse Gear:**
    *   **Likely ID:** `0x0F6` (Source: BSI) - Often contains a bit indicating the reverse gear light is active.
    *   **`canbox` Need:** Decode the reverse gear status to control the `REAR` output signal (using the configured delay logic in `main.c`) and update internal state (`carstate.selector`). *Note: Gear selector position (P,N,D,S,M) might be on a different bus or message ID.*
*   **Doors/Trunk/Bonnet Status:**
    *   **Likely ID:** `0x131` (Source: BSI) - Often contains status bits for doors.
    *   **Likely ID:** `0x220` (Source: BSI) - Another common ID for door status.
    *   **`canbox` Need:** Decode individual bits for each door, tailgate, and bonnet to update `carstate` (`fl_door`, `fr_door`, etc.) for potential display via the head unit protocol.
*   **Vehicle Speed:**
    *   **Likely ID:** `0x14C` / `0x28C` (Source: BSI/ABS?) - Often contains vehicle speed, potentially duplicated.
    *   **`canbox` Need:** Decode speed value (often needs scaling, e.g., * 0.01) to update `carstate.speed`.
*   **Odometer:**
    *   **Likely ID:** `0x14C` / `0x28C` (Source: BSI?) - Often paired with speed.
    *   **`canbox` Need:** Decode odometer value (check byte order and potential scaling) to update `carstate.odometer`.
*   **Steering Wheel Controls:**
    *   **Possible IDs:** `0x126`, `0x21F` (from `PSACAN.md`), `0x1B0` (from RCZ/Headunit code - *less likely* for direct car info but possible for head unit interaction). **This requires careful sniffing during button presses.**
    *   **`canbox` Need:** Decode button press/release events and map them to the `key_cb_t` callbacks (`inc_volume`, `dec_volume`, `prev`, `next`, `mode`, etc.) in `car.c`.
*   **Parking Sensors (Parktronic):**
    *   **Likely ID:** `0x0E1` (Source: Parktronic ECU)
    *   **`canbox` Need:** Decode sensor distances (front/rear, individual sensors) and the active state. Update `carstate.radar`. The Hiworld protocol expects specific distance formatting.
*   **Outside Temperature:**
    *   **Likely ID:** `0x168` (Source: BSI/Sensor)
    *   **`canbox` Need:** Decode temperature value (often needs scaling/offset) to update `carstate.temp`.
*   **Battery Voltage:**
    *   **Likely ID:** `0x168` (Source: BSI)
    *   **`canbox` Need:** Decode voltage (often needs scaling) to update `carstate.voltage`.
*   **VIN:**
    *   **Likely IDs:** `0x2B6`, `0x336`, `0x3B6` (Source: BSI) - Often split across multiple messages.
    *   **`canbox` Need:** Assemble the full VIN in `carstate.vin`.
*   **Climate Control (Basic):**
    *   **Likely IDs:** `0x1D0`, `0x260`, `0x261` (Source: Climate ECU)
    *   **`canbox` Need (Basic):** Decode essential status like AC On/Off, Recirculation, Defrost states for potential display via head unit protocol. Update `car_air_state`.

### 4. `canbox` Firmware Implementation Steps for PF2 (Peugeot 407)

1.  **Create/Update `cars/peugeot_407.c`:**
    *   Define `msg_desc_t peugeot_407_ms[]` array.
    *   For each essential CAN ID identified above:
        *   Add an entry to the `peugeot_407_ms` array specifying the `id`, estimated `period` (from logs), and the handler function name (e.g., `{ 0x036, 100, 0, 0, peugeot_407_ms_036_ign_light_handler }`).
        *   Implement the corresponding handler function (`static void peugeot_407_ms_XXX_handler(const uint8_t * msg, struct msg_desc_t * desc)`).
        *   Inside the handler:
            *   Check for timeouts using `is_timeout(desc)` and set corresponding `carstate` members to default/inactive values if timed out.
            *   Decode the relevant bits/bytes from the `msg` buffer according to your verified CAN data format.
            *   Apply necessary masks, shifts, scaling factors, and offsets.
            *   Update the appropriate members of the `carstate` or `car_air_state` structs.
            *   For steering wheel buttons, call the relevant functions from `key_state.key_cb` (e.g., `key_state.key_cb->inc_volume(1);`).
2.  **Update `src/car.c`:**
    *   Ensure `#define USE_PEUGEOT_407 1` (or similar) is active, or use the build flag `-DUSE_PEUGEOT_407=y`.
    *   Add `#include "cars/peugeot_407.c"` within the appropriate `#ifdef`.
    *   In `car_init()`:
        *   Add a case for `e_car_peugeot_407`.
        *   Set the correct CAN speed: `hw_can_set_speed(hw_can_get_mscan(), e_speed_125);`
    *   In `car_process()`:
        *   Add a case for `e_car_peugeot_407` that calls `in_process(can, ticks, peugeot_407_ms, sizeof(peugeot_407_ms)/sizeof(peugeot_407_ms[0]));`.
3.  **Update `include/conf.h`:**
    *   Add `e_car_peugeot_407` to the `enum e_car_t`.
4.  **Update `src/main.c`:**
    *   In `print_debug()`, add a case to the `switch (car)` statement to print "Peugeot 407 2008MY" (or similar) when `car == e_car_peugeot_407`.

### 5. Firmware Configuration

*   **Select Car Model:** Once flashed, connect via serial (`tio`, `minicom`) and enter debug mode (`OOOOOOOOOOOO`). Use the `c` command repeatedly until "PEUGEOT 407 2008MY" (or your chosen name) is displayed.
*   **Select Head Unit Protocol:** Use the `b` command repeatedly to select the desired protocol (e.g., "Hiworld PSA").
*   **Save Configuration:** Press `s` to save the settings to the CAN box's non-volatile memory.
*   **Exit Debug Mode:** Press `o`.

### 6. Testing and Debugging on PF2

*   **CAN Sniffing (Crucial):** Use SavvyCAN (or `cansniffer`/`candump`) connected to the *actual* Peugeot 407's CAN Comfort bus to:
    *   Verify the exact IDs for ignition, lights, doors, steering wheel buttons, reverse, etc.
    *   Log data while performing actions (pressing buttons, shifting, opening doors) to understand the data bytes.
*   **Renode + GDB:**
    *   Use `canplayer` or `cansend` to inject *verified* CAN messages into `vcan0` while running the firmware in Renode.
    *   Set breakpoints in your `peugeot_407_ms_XXX_handler` functions.
    *   Step through the code, inspect the `msg` data, and verify that `carstate` members are updated correctly.
    *   Check for correct scaling and offset calculations.
*   **Serial Monitoring:**
    *   Use `tio` or the `logger.py` script connected to the Renode UART PTY (`/tmp/uart`) or the physical serial port (`/dev/ttyUSB2` when testing with the head unit) to monitor the outgoing Hiworld PSA messages (`5A A5 ...`).
    *   Verify that the CANbox sends the correct Hiworld messages based on the simulated CAN input and the state changes in `carstate`.

### 7. Important Considerations for PF2

*   **Message ID Verification:** Cannot stress this enough – do not assume IDs from other models (even other PSA cars) or documentation are correct without verifying on *your* 407.
*   **Data Byte Mapping:** The exact bit/byte used for a specific signal can vary. Careful analysis of logged data is required.
*   **BSI Centrality:** The BSI is the source or destination for many Comfort CAN messages. Understanding its role can help in troubleshooting.
*   **Timeout Logic:** Ensure your handlers correctly use `is_timeout()` to revert `carstate` values to a safe/default state (e.g., doors closed, ACC off) if a message stops being received.
*   **Head Unit Protocol:** The success depends equally on correctly implementing the CAN decoding (`peugeot_407.c`) *and* the head unit serial protocol (`protocol/hiworldpsa.c`).

By following these steps and focusing on verification through CAN sniffing and Renode debugging, you can successfully adapt the `canbox` firmware for your Peugeot 407.