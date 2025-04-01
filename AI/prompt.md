Hello! I'd like to continue working on the `canbox-renode` firmware project.

**Project Goal:** To create a functional open-source firmware for STM32F1-based CAN boxes (like the Volvo OD2 adapter) acting as a proxy between a Peugeot 407 (2008MY, PF2 Platform, CAN Comfort @ 125kbps) and a generic Android Head Unit (QF001/ROCO K706, UIS7862a/s).

**Target Head Unit Protocol:** We are currently implementing and testing the **[Choose ONE and specify: Hiworld PSA OR RZC PSA]** protocol based on the documentation previously provided ([Specify which doc: input.md OR converted_document.md]). The target serial baud rate for this protocol is **[Specify: 38400 OR 19200]** bps.

**Current Status (as of last session):**

*   We have implemented CAN handlers in `cars/peugeot_407.c` for the following verified CAN IDs: [List confirmed/implemented IDs, e.g., 0x036, 0x0F6, 0x131, 0x21F, 0x161, 0x0E1, 0x168, 0x28C, 0x2A1, 0x261].
*   We have implemented parts of the chosen serial protocol (`[protocol/hiworldpsa.c OR protocol/raiserzc.c]`) to send basic vehicle status ([List features implemented, e.g., doors, basic lights, temp, voltage, some trip data]).
*   We identified the need to verify/implement handlers for [List remaining IDs or features from the priority list, e.g., Trip 1/2 scaling, specific SWC buttons, Park Brake state].
*   The development environment uses Renode for simulation/debugging, PlatformIO/Make for building, and `tio`/`cansend`/`candump` for interaction.
*   We have a workbench setup with the physical CAN box, head unit, and CAN interface.

**Attached:**

*   A ZIP file containing the current state of the `canbox-renode` project repository.

**Task for This Session:**

[**Clearly state what you want to work on next.** Examples:]

*   "Please help me implement the CAN handler for the Park Brake signal. I have logs showing CAN ID `0xXXX` changes when the brake is applied/released."
*   "Let's refine the temperature scaling for CAN ID `0x168` based on my latest workbench measurements."
*   "Help me implement the sending logic for Trip Computer data (Pages 1 and 2) using the Hiworld PSA ComIDs `0x14` and `0x15` based on the data now available in `carstate`."
*   "Review the current `protocol/hiworldpsa.c` file and suggest improvements or point out potential issues based on the `input.md` specification."
*   "Generate Robot Framework test cases for the recently implemented Parking Sensor feature (`0x0E1` handler and Hiworld `0x41` output)."
*   "I'm seeing an issue where the `ILLUM` pin is not turning off correctly. Let's debug the logic in `peugeot_407_ms_036_ign_light_handler` and `gpio_process`."

Please analyze the attached project state and assist me with the task described above.
