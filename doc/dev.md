# DETAILED_DOCUMENTATION.md

## Project: CANBUS Box Firmware (Reverse Engineered and Enhanced)

This project provides reverse-engineered and enhanced firmware for commonly found "CANBUS boxes" or "CANBUS decoders" designed to interface between a vehicle's CAN bus and an Android head unit. These adapters are often found on AliExpress and similar platforms. The original firmware often has limitations or lacks support for certain vehicle features.  This project aims to provide a more flexible, configurable, and feature-rich alternative.

**Project Goal:** To create open-source, customizable firmware for STM32F1 and Nuvoton NUC131 based CANBUS adapters, allowing users to extract more data from their vehicle's CAN bus and integrate it with Android head units.

**Supported Hardware:**

*   **Volvo OD2 Adapter:** Based on the STM32F103x8 microcontroller. (Makefile_volvo_od2)
*   **VW NC03 Adapter:** Based on the Nuvoton NUC131 microcontroller. (Makefile_vw_nc03, NUC131_BSP_DIR)
*   **VW_NCD01 Adapter:** Likely a newer version of VW_NC03, also using a Nuvoton microcontroller. (Inferred from the project files).
*   **QEMU Emulation:** Support for running and debugging the STM32F1 version within QEMU.

**Supported MCU Families:**

*   STMicroelectronics STM32F1 series (specifically STM32F103x8)
*   Nuvoton NUC131 series

**Supported CANBUS Protocols (Emulated):**

The firmware *emulates* the following CANBUS protocols, typically used by Android head units to communicate with vehicle CAN adapters:

*   **Raise VW (PQ)**
*   **Raise VW (MQB)**
*   **Audi BMW (NBT Evo)**
*   **HiWorld VW (MQB)**
*   **Mercedes-Benz UDS-based CANBUS** (mentioned in README.md)

**Supported Car Models (Confirmed):**

* Land Rover Freelander 2 (2007MY, 2013MY)
* Volvo XC90 (2007MY)
* Volkswagen Tiguan (implied by VW_NC03 adapter)
* Skoda Fabia (mentioned in code)
* Audi Q3 2015 (mentioned in code)
* Toyota Premio 26x (mentioned in code)
*   Other vehicles using similar CANBUS adapters (general support)

**Core Functionality:**

The firmware performs the following key functions:

1.  **CAN Bus Data Extraction:** Reads and decodes messages from the vehicle's CAN bus (typically MS-CAN, but adaptable).  The specific CAN IDs and data formats are car-model-specific, and the `cars/` directory contains implementations for supported vehicles.
2.  **Signal Generation (for Head Unit):**  Generates the standard analog signals required by many Android head units:
    *   **IGN/ACC (Ignition/Accessory):**  12V signal when the ignition is on.  Powers the head unit.  This is a critical signal.
    *   **ILLUM (Illumination):**  12V signal that typically controls backlight brightness based on headlight or ambient light status.
    *   **PARK (Parking Brake):**  Ground signal when the parking brake is engaged.  Often used to restrict video playback while driving.
    *   **REAR (Reverse Gear):** 12V signal when reverse gear is selected.  Used to activate the rearview camera and supply power to it. Configurable delays are implemented to handle momentary disengagements of reverse gear.
3.  **CANBUS Protocol Emulation (for Head Unit):**  Communicates with the Android head unit over a serial connection (RX/TX, UART, typically at 38400 baud) using one of the emulated CANBUS protocols (Raise, HiWorld, etc.).  This allows the head unit to display vehicle information and receive button presses.
4.  **User Configuration and Debugging:**  Provides a serial-based command interface (accessible via RX/TX lines) for:
    *   Switching between normal operation and debug mode.
    *   Selecting the emulated CANBUS protocol.
    *   Selecting the vehicle model.
    *   Adjusting parameters (e.g., backlight threshold, camera delay).
    *   Saving the configuration to non-volatile memory (where supported).
    *   Monitoring CAN bus traffic (sniffer mode).

## Software Architecture

The firmware is structured in a modular way to facilitate adding support for new vehicles and CANBUS protocols.

### Main Components

*   **`main.c`**: The main application loop.  It initializes the hardware, handles the main processing loop, and calls functions from other modules.
*   **`hw.c`, `hw.h`**:  Hardware Abstraction Layer (HAL).  Provides a consistent interface to the microcontroller peripherals (GPIO, USART, CAN, timers, etc.) regardless of the specific MCU (STM32F1 or NUC131).  This is where the platform-specific code is isolated.
*   **`hw_*.c`, `hw_*.h`**: Individual HAL implementations for specific peripherals (e.g., `hw_can.c`, `hw_usart.c`, `hw_tick.c`, `hw_gpio.c`).
*   **`canbox.c`, `canbox.h`**: Implements the communication with the Android head unit over the serial interface (RX/TX).  This module handles the emulated CANBUS protocols (Raise VW, etc.). It sends data to the head unit and processes commands received from the head unit.
*   **`car.c`, `car.h`**:  Contains the vehicle-specific logic for decoding CAN bus messages.  This is the most important part to customize for different car models.  It provides a standardized interface for accessing vehicle data (e.g., `car_get_speed()`, `car_get_door_fl()`).
*   **`conf.c`, `conf.h`**:  Handles reading and writing configuration data (e.g., selected vehicle, CANBUS protocol, delays) to non-volatile storage.  The `hw_conf.h` file provides the platform-specific details for accessing the storage (e.g., flash memory addresses).
*   **`ring.c`, `ring.h`**:  Implements a ring buffer (circular buffer) for efficient handling of serial data.

### Subdirectories

*   **`cars/`**: This directory contains files that are *specific to individual car models*.  Each car model has its own `.c` file (e.g., `lr2_2007my.c`, `xc90_2007my.c`, `skoda_fabia.c`). These files implement the CAN message handling for that specific vehicle.  They define:
    *   `msg_desc_t` structures:  These structures describe the CAN messages to be monitored, their IDs, periods, and handler functions.
    *   Handler functions:  These functions are called when a matching CAN message is received.  They decode the data from the CAN message and update the `carstate` structure with the relevant vehicle information (e.g., speed, gear, door status).

*   **`qemu/`, `volvo_od2/`, `vw_nc03/`**: These directories contain platform-specific files (e.g., linker scripts, hardware initialization code) for QEMU, the Volvo OD2 adapter, and the VW NC03 adapter, respectively.

*   **`libopencm3/`**:  This directory contains the libopencm3 library, which provides low-level hardware access for STM32 microcontrollers.  The `include/` directory contains headers, and the `lib/` directory contains compiled libraries.

### Data Flow

1.  **CAN Bus Reception:** The `hw_can` module receives CAN messages from the vehicle's bus.
2.  **CAN Message Filtering and Handling:**  The `car_process()` function (in `car.c`) is called periodically. It retrieves received CAN messages from the `hw_can` module and uses the `msg_desc_t` structures (defined in the car-specific files) to identify and process relevant messages.
3.  **Vehicle State Update:**  The CAN message handler functions (e.g., `lr2_2007my_ms_6c_handler` in `cars/lr2_2007my.c`) decode the data from the CAN messages and update the `carstate` structure (in `car.c`) with the current vehicle state.
4.  **CAN Box Signal Generation:** The `main()` function periodically calls `gpio_process()`, which uses the information in `carstate` to control the output signals (IGN, ILLUM, REAR, PARK) to the head unit.
5.  **CAN Box Communication:** The `canbox_process()` function (in `canbox.c`) is called periodically. It handles communication with the Android head unit via the serial interface (RX/TX). It uses the `carstate` structure to provide vehicle data to the head unit, and it processes commands received from the head unit.
6.  **User Configuration:** The `conf_read()` and `conf_write()` functions (in `conf.c`) manage persistent configuration data.
7.  **Debug Mode:**  The `debug_ch_process()` function (in `main.c`) handles serial commands in debug mode, allowing configuration changes and monitoring of internal variables.

## Building and Flashing

The project uses Makefiles for building and flashing the firmware.

**General Build Process:**

1.  **Install Prerequisites:**  You'll need a suitable ARM cross-compiler toolchain (e.g., `arm-none-eabi-gcc`), Make, and (for flashing) OpenOCD. The `README.md` file provides installation instructions for Ubuntu.
2.  **Navigate to Project Directory:** Open a terminal and navigate to the root directory of the project (where the main `Makefile` is located).
3.  **Build:** Use the `make` command with the appropriate target:
    *   `make`: Builds all targets (vw_nc03.bin, volvo_od2.bin, qemu.bin).
    *   `make vw_nc03.bin`: Builds the firmware for the VW NC03 adapter.
    *   `make volvo_od2.bin`: Builds the firmware for the Volvo OD2 adapter.
    *   `make qemu.bin`: Builds the firmware for QEMU emulation.
4.  **Flash:** Use the `make flash_...` commands:
    *   `make flash_vw_nc03`: Flashes the firmware to a Raise VW NC03 adapter using OpenOCD.  Requires an ST-Link V2 programmer.
    *   `make flash_volvo_od2`: Flashes the firmware to a Raise Volvo OD2 adapter using OpenOCD. Requires an ST-Link V2 programmer.

**QEMU Emulation:**

*   **Build for QEMU:** `make qemu`
*   **Run in QEMU:** `make run_qemu` (this launches QEMU with the compiled `qemu.bin`).
*   **Debugging with GDB:**
    1.  Start QEMU in debug mode (with `-S -s` options) in one terminal:

        ```bash
        qemu-system-arm -M stm32-p103 -kernel firmware.elf -nographic -S -s
        ```
        or
        ```
        make run_qemu 
        ```
    2.  Start a GDB session in another terminal:
        ```bash
        arm-none-eabi-gdb firmware.elf
        ```
    3.  Connect GDB to QEMU:
        ```
        target remote :1234
        ```
    4.  Set breakpoints, continue execution, inspect variables, etc.

## File Descriptions (Detailed)

This section provides more in-depth descriptions of the key source files.

*   **`main.c`**:
    *   Initializes the hardware using `hw_setup()`.
    *   Reads the configuration from flash using `conf_read()`.
    *   Initializes the vehicle-specific logic using `car_init()`.
    *   Enters the main loop, which:
        *   Calls `gpio_process()` to update output signals (ACC, ILLUM, REAR, PARK).
        *   Calls `usart_process()` to handle serial communication with the head unit.
        *   Periodically (based on timer flags) calls `car_process()` to update vehicle state and `canbox_park_process()`/`canbox_process()` to send data to the head unit.
        *   Handles sleep mode entry.
    *    `rear_delay_process`: Implements logic for delays of reverse gear
    *    `debug_ch_process`: handles UART commands, if in debug mode
    *    `usart_process`     : receives commands via UART if in debug mode
    *    `print_sniffer`    : prints CAN messages.
    *    `print_debug`      : print all states, configuration, and CAN data

*   **`canbox.c`, `canbox.h`**:
    *   `canbox_checksum()`: Calculates a simple checksum for CANBUS messages.
    *   `snd_canbox_msg()`: Sends a CANBUS message to the head unit (over UART).
    *   `canbox_process()`: The main processing function for head unit communication. This is called periodically and handles sending data to the head unit according to the selected protocol.  It calls functions like `canbox_raise_vw_wheel_process()`, `canbox_raise_vw_door_process()`, etc. to prepare the data to be sent.
    *   `canbox_park_process()`: Similar to `canbox_process()`, but specifically for radar data (parking sensors).
    *   `canbox_cmd_process()`: Handles commands received from the head unit (in Raise/Hiworld protocol format).
    *   `canbox_raise_vw_*`, `canbox_hiworld_vw_mqb_*`: Functions specific to each supported CAN box protocol (Raise, Hiworld).

*   **`car.c`, `car.h`**:
    *   `car_state_t`:  A structure that holds the current state of the vehicle (ignition, gear, doors, etc.).  This is the central repository for vehicle data.
    *   `car_air_state_t`: state of the air condition
    *   `msg_desc_t`:  Describes a CAN message to be monitored, including its ID, period, and a handler function.
    *   `in_process()`:  Iterates through received CAN messages and calls the appropriate handler function for each message.
    *   `car_init()`:  Initializes the `carstate` structure and sets the CAN bus speed based on the selected car.
    *   `car_process()`:  Calls `in_process()` to handle incoming CAN messages, updating the vehicle state.  This is the core function for processing vehicle CAN data.
    *   `car_get_*()`:  A set of getter functions to retrieve specific vehicle data from the `carstate` structure (e.g., `car_get_acc()`, `car_get_speed()`, `car_get_door_fl()`).
    *   `car_get_next_car()`: returns next supported car

*   **`conf.c`, `conf.h`**:
    *   `conf_t`:  A structure that holds the configuration settings (selected car, CANBUS protocol, backlight level, camera delay).
    *   `conf_read()`:  Reads the configuration from non-volatile storage (flash memory).
    *   `conf_write()`: Writes the configuration to non-volatile storage.
    *   `conf_get_*()`, `conf_set_*()`: Getter and setter functions for configuration parameters.

*   **`hw.c`, `hw.h`**:
    *   `hw_setup()`:  Initializes all hardware components (clock, GPIO, USART, CAN, SysTick).
    *   `hw_sleep()`:  Puts the microcontroller into a low-power sleep mode.
    *   `hw_gpio_setup()`, `hw_gpio_disable()`, `hw_gpio_rear_on()`, etc.:  Functions to control GPIO pins (for signals like IGN, ILLUM, REAR).

*   **`hw_can.c`, `hw_can.h`**:
    *   `can_t`:  A structure that holds the CAN peripheral configuration and state.
    *   `hw_can_setup()`:  Initializes the CAN peripheral (baud rate, etc.).
    *   `hw_can_disable()`:  Disables the CAN peripheral.
    *   `hw_can_get_msg_nums()`:  Returns the number of CAN messages received.
    *   `hw_can_get_pack_nums()`:  Returns the total number of CAN packets received.
    *   `hw_can_get_msg()`:  Retrieves a received CAN message.
    *   `hw_can_snd_msg()`:  Transmits a CAN message.
    *   `hw_can_clr()`: Clears the received messages buffer
    *   `hw_can_sleep()`: Puts the CAN peripheral into a low-power state.

*   **`hw_tick.c`, `hw_tick.h`**:
    *   `hw_systick_setup()`:  Configures the SysTick timer for periodic interrupts.
    *   `hw_systick_disable()`:  Disables the SysTick timer.
    *   `hw_systick_callback()`:  The SysTick interrupt handler.  This updates the `timer` structure, which tracks time in milliseconds and seconds and sets flags for periodic events.
    *   `timer`: global variable to update tick flags

*   **`hw_usart.c`, `hw_usart.h`**:
    *   `usart_t`:  A structure that holds the USART peripheral configuration and state.
    *   `hw_usart_setup()`:  Initializes the USART peripheral (baud rate, etc.).
    *   `hw_usart_disable()`:  Disables the USART peripheral.
    *   `hw_usart_write()`:  Writes data to the USART transmit buffer.
    *   `hw_usart_read_ch()`:  Reads a character from the USART receive buffer.
    *   `usart_isr()`:  The USART interrupt handler.  Handles receiving and transmitting data using ring buffers.

*   **`ring.c`, `ring.h`**:
    *   `ring_t`:  A structure that defines a ring buffer.
    *   `ring_init()`:  Initializes a ring buffer.
    *   `ring_write_ch()`:  Writes a single character to the ring buffer.
    *   `ring_write()`:  Writes a block of data to the ring buffer.
    *   `ring_read_ch()`:  Reads a single character from the ring buffer.
    *   `ring_get_overflow()`: return ring buffer overflow counter

*   **`sbrk.c`**:
     *   `_sbrk_r()`: Implements the `sbrk` system call, which is used by `malloc` to allocate memory on the heap.
     *   `__initialize_args()`:  A weak function that can be overridden to provide command-line arguments to the program (used for semihosting).

*   **`cars/*.c`**:  Car-specific CAN message handling.  These files contain the logic to decode CAN messages from specific vehicle models.
*   **`Makefile*`**: The makefiles

## Key Concepts

*   **Hardware Abstraction Layer (HAL):** The `hw_*.c` and `hw_*.h` files provide a hardware abstraction layer, separating the core firmware logic from the specific microcontroller peripherals. This makes it easier to port the firmware to different STM32 or Nuvoton chips.

*   **CAN Bus Communication:** The firmware interacts with the vehicle's CAN bus using the `hw_can` module.  It receives and decodes CAN messages to extract relevant vehicle data.

*   **CANBUS Protocol Emulation:** The `canbox.c` module emulates the communication protocols used by common Android head units. This allows the firmware to "pretend" to be a standard CANBUS adapter, making it compatible with a wide range of head units.

*   **Ring Buffers:** The `ring.c` and `ring.h` files implement ring buffers (circular buffers) for efficient handling of data streams, particularly for the serial communication with the head unit.

*   **Configuration:** The `conf.c` and `conf.h` files handle persistent storage of configuration settings, allowing users to customize the firmware's behavior.

*   **Debugging:** The firmware includes a debug mode that allows for interaction and configuration via a serial terminal.

## How to Use This Documentation

1.  **Start with `main.c`:** This is the entry point of the application and provides a good overview of the program flow.
2.  **Explore the `hw` files:** Understand the hardware abstraction layer and how it interacts with the specific microcontroller peripherals.
3.  **Dive into `car.c` and the `cars/` directory:**  This is where the vehicle-specific CAN message decoding happens.  If you're adapting the firmware to a new vehicle, this is the most important part to modify.
4.  **Understand `canbox.c`:**  This is where the communication with the Android head unit is handled, including the emulation of various CANBUS protocols.
5.  **Use the Makefiles:** Understand the different build targets and how to compile and flash the firmware.
6.  **Reference `libopencm3`:**  For detailed information on the STM32-specific peripheral functions (from `libopencm3`), refer to the libopencm3 documentation.

This detailed documentation should provide a comprehensive understanding of the project's structure, functionality, and how to adapt it to different vehicles and configurations. Remember to consult the specific datasheets and reference manuals for the STM32F1 and Nuvoton NUC131 microcontrollers for low-level hardware details.

## VS Code Configuration for Debugging

This project includes a `launch.json` file in the `.vscode` directory for easy debugging within VS Code. It supports both on-hardware debugging (using OpenOCD and an ST-Link V2 programmer) and QEMU emulation.

### `launch.json` Configurations

The `.vscode/launch.json` file defines the following debug configurations:

1.  **`OpenOCD (STM32 - Volvo OD2)`:**
    *   **Purpose:** This configuration is used to debug the firmware running on the STM32F103x8 microcontroller found in the Volvo OD2 adapter.
    *   **Debugger:** It uses OpenOCD as the debugging server.
    *   **Hardware:** Requires an ST-Link V2 programmer connected to the target board.
    *   **Configuration Files:** It specifies the `interface/stlink.cfg` (for the ST-Link V2 interface) and `target/stm32f1x.cfg` (for the STM32F103x8 target).
    *   **Pre-launch task:** The `build_volvo_od2` build task is ran before starting the debug session.
    * **Post-debug task:** The `terminate_openocd` task is ran after the session is stopped.
    *   **Executable:**  `firmware.elf`

2.  **`OpenOCD (Nuvoton NUC131 - VW NC03)`:**
    *   **Purpose:**  This configuration is used to debug the firmware running on the Nuvoton NUC131 microcontroller found in the VW NC03 adapter.
    *   **Debugger:** It uses OpenOCD as the debugging server.
    *   **Hardware:** Requires an ST-Link V2 programmer connected to the target board.
    *   **Configuration Files:** It specifies the `interface/stlink.cfg` (for the ST-Link V2 interface) and `target/NUC131.cfg` (for the NUC131 target). You will need to create the `target/NUC131.cfg` file.
    *   **Pre-launch task:** The `build_vw_nc03` build task is ran before starting the debug session.
    * **Post-debug task:** The `terminate_openocd` task is ran after the session is stopped.
    *   **Executable:**  `firmware.elf`

3.  **`QEMU (STM32)`:**
    *   **Purpose:** This configuration is used to debug the STM32F1 firmware within the QEMU emulator.
    *   **Debugger:** It uses `arm-none-eabi-gdb` (the ARM cross-compiler's GDB) connected to QEMU.
    *   **Emulation:** QEMU must be running separately, acting as a GDB server.
    *   **GDB Server Address:** It connects to `localhost:1234`, which is the default port QEMU uses for GDB.
    *   **Pre-launch task:** The `build_qemu` build task is ran before starting the debug session.
    * **Post-debug task:** The `run_qemu` task is ran after the session is stopped.
    *   **Executable:**  `firmware.elf`

### `tasks.json` and Build Tasks

The `launch.json` file uses `preLaunchTask` and `postDebugTask` properties to define tasks that should be run before starting a debug session and after the session is stopped. These tasks are defined in the `.vscode/tasks.json` file.

The following tasks are defined:

* **`build_volvo_od2`**: Builds the `firmware.elf` file for the Volvo OD2 target. It uses the `make volvo_od2.bin` command.
* **`build_vw_nc03`**: Builds the `firmware.elf` file for the VW NC03 target. It uses the `make vw_nc03.bin` command.
* **`build_qemu`**: Builds the `firmware.elf` file for the QEMU target. It uses the `make qemu.bin` command.
* **`terminate_openocd`**: This task terminates any running openocd process.
* **`run_qemu`**: run the QEMU target, with the proper parameters.

```
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "build_volvo_od2",
            "type": "shell",
            "command": "make",
            "args": ["volvo_od2.bin"],
            "group": {
                "kind": "build",
                "isDefault": true
            },
            "problemMatcher": []
        },
        {
            "label": "build_vw_nc03",
            "type": "shell",
            "command": "make",
            "args": ["vw_nc03.bin"],
            "group": {
                "kind": "build",
                "isDefault": true
            },
            "problemMatcher": []
        },
        {
            "label": "build_qemu",
            "type": "shell",
            "command": "make",
            "args": ["qemu.bin"],
            "group": {
                "kind": "build",
                "isDefault": true
            },
            "problemMatcher": []
        },
        {
            "label": "terminate_openocd",
            "type": "shell",
            "command": "pkill",
            "args": ["openocd"],
            "problemMatcher": []
        },
        {
            "label": "run_qemu",
            "type": "shell",
            "command": "make",
            "args": ["run_qemu"],
            "problemMatcher": []
        }
    ]
}

```

### How to Use the VS Code Debugger

1.  **Install Extensions:**
    *   Ensure you have the **Cortex-Debug** extension installed and enabled.
    *   Ensure you have the **C/C++** extension installed and enabled.
2.  **Install Toolchain:**
    *   Ensure you have the **ARM cross-compiler toolchain** (including `arm-none-eabi-gdb`) installed and added to your system's PATH.
    *   Ensure you have **OpenOCD** installed and added to your system's PATH.
    * Ensure you have **QEMU** installed.
3.  **Build the Firmware:**
    *   Use the `make` command in the terminal to build the appropriate firmware target (e.g., `make volvo_od2.bin`, `make vw_nc03.bin`, or `make qemu.bin`).
    * This build will generate the `firmware.elf` file.
4.  **Connect Hardware (If Applicable):**
    *   For on-hardware debugging (STM32 or Nuvoton), connect your ST-Link V2 programmer to the target board.
5.  **Open VS Code:**
    *   Open the project folder in VS Code.
6.  **Select Debug Configuration:**
    *   Go to the "Run and Debug" view (Ctrl+Shift+D or Cmd+Shift+D).
    *   Select the desired configuration from the dropdown menu (e.g., "OpenOCD (STM32 - Volvo OD2)," "OpenOCD (Nuvoton NUC131 - VW NC03)," or "QEMU (STM32)").
7.  **Start Debugging:**
    *   Press the "Start Debugging" button (or F5).
    * The build task will be executed first.
    * The debug process will start.
8. **QEMU specific**:
    * If you are using the "QEMU (STM32)", the debug process will also execute the `run_qemu` task. You can also execute it manually, in a terminal.
9.  **Set Breakpoints, Inspect, etc.:**
    *   Use the standard VS Code debugger features to set breakpoints, step through code, inspect variables, etc.

### Troubleshooting

*   **"Property server is not allowed":** Ensure the "Cortex-Debug" extension is correctly installed and enabled in VS Code.
* **"Please use type node instead" and  "The debug type is not recognized"**: Ensure that the "Cortex-Debug" and "C/C++" extensions are enabled.
*   **OpenOCD Errors:** Check your OpenOCD installation and make sure the interface and target configuration files are correct.
* **target/NUC131.cfg missing**: This file is needed for Nuvoton debugging, you will have to create it.
*   **GDB Connection Errors (QEMU):** Ensure that QEMU is running in debug mode (`make run_qemu` or manually with `-S -s`) and that GDB is configured to connect to the correct port (usually `localhost:1234`).
* **Wrong OpenOCD/GDB**: Verify that the correct version of openocd and GDB is used.
* **Wrong PATH**: Check that all required commands are in your PATH.

This documentation should provide you with the necessary information to use VS Code for debugging this project.

## VS Code Configuration for Debugging

This project includes a `launch.json` file in the `.vscode` directory for easy debugging within VS Code. It supports both on-hardware debugging (using OpenOCD and an ST-Link V2 programmer) and QEMU emulation.

### `launch.json` Configurations

The `.vscode/launch.json` file defines the following debug configurations:

```jsonc
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "OpenOCD (STM32 - Volvo OD2)",
            "type": "cortex-debug",
            "request": "launch",
            "cwd": "${workspaceFolder}",
            "preLaunchTask": "build_volvo_od2",
            "postDebugTask": "terminate_openocd",
            "configFiles": [
                "interface/stlink.cfg",
                "target/stm32f1x.cfg"
            ],
            "executable": "${workspaceFolder}/firmware.elf"
        },
        {
            "name": "OpenOCD (Nuvoton NUC131 - VW NC03)",
            "type": "cortex-debug",
            "request": "launch",
            "cwd": "${workspaceFolder}",
            "preLaunchTask": "build_vw_nc03",
            "postDebugTask": "terminate_openocd",
            "configFiles": [
                "interface/stlink.cfg",
                "target/NUC131.cfg"
            ],
            "executable": "${workspaceFolder}/firmware.elf"
        },
        {
            "name": "QEMU (STM32)",
            "type": "cppdbg",
            "request": "launch",
            "cwd": "${workspaceFolder}",
            "program": "${workspaceFolder}/firmware.elf",
            "miDebuggerServerAddress": "localhost:1234",
            "miDebuggerPath": "arm-none-eabi-gdb",
            "preLaunchTask": "build_qemu",
            "postDebugTask": "run_qemu",
            "MIMode": "gdb"
        }
    ]
}
