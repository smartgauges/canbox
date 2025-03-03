# CANBUS Box Firmware - Reverse Engineered and Enhanced

This project is a significantly reworked and expanded version of the [canbox](https://github.com/smartgauges/canbox) project by smartgauges, providing reverse-engineered and enhanced firmware for STM32F1 and Nuvoton NUC131-based CANBUS adapters (often called "CANBUS boxes" or "decoders"), commonly used to interface between a vehicle's CAN bus and an Android head unit.  These adapters are readily available on platforms like AliExpress. This project aims to deliver open-source, configurable, and feature-rich firmware, going beyond the often-limited capabilities of the original firmware.

**AI Assistance Disclaimer:**  This project, including the code, documentation (like this README), and overall structure, has been heavily developed and enhanced with the assistance of an AI language model. Specifically, **Google AI's Gemini Pro** was used extensively for:

*   Code generation and modification
*   Documentation writing and organization
*   Content enhancement based on original sources (including forum comments)
*   Creation of build and debugging configurations
*   Translation and summarization of the original Russian documentation.

**Project Goal and Roadmap:**

The primary goal is to create open-source, customizable firmware for STM32F1 and Nuvoton NUC131-based CANBUS adapters. This enables users to extract more data from their vehicle's CAN bus and seamlessly integrate it with Android head units.  A key part of making this project easier to develop and extend is the inclusion of robust emulation and debugging environments.

**Current Status:**

*   The original project provides a functional base for STM32F1 (Volvo OD2 adapter) and Nuvoton NUC131 (VW NC03, VW_NCD01 adapters).
*   QEMU emulation is already available and working for the *STM32F1* version.
*   Extensive documentation and build/debug configurations (for VS Code and command-line tools) have been added.

**Future Plans (Roadmap):**

1.  **Enhanced QEMU Environment:** Expand the existing QEMU emulation environment to include simulated CAN bus traffic, mimicking both the *vehicle's* CAN bus and the *Android head unit's* serial communication. This will enable a comprehensive, hardware-free development and testing workflow.
2. **Nuvoton NUC131 QEMU support**: Add QEMU support, which will require creating QEMU device for Nuvoton NUC131.
3.  **Modular CAN Protocol Emulation:** Further refine the `canbox.c` module to make it easier to add and switch between different emulated head unit protocols.
4.  **Expanded Vehicle Support:**  Encourage community contributions to add support for more car models by providing clear guidelines and templates in the `cars/` directory.
5.  **Improved Configuration:**  Potentially develop a more user-friendly configuration method (e.g., a simple web interface or configuration file) instead of relying solely on the serial command interface.
6.  **Head Unit Communication Protocol Reverse Engineering:** Continue to improve the understanding and implementation of various head unit communication protocols (Raise, HiWorld, etc.) by analyzing their behavior and referring to available open-source projects.
7.	**Support for more MCU Families:** Add support for more MCU families
8. **Support for more CAN Box protocols:** Add support for even more headunit communication protocols.

**Key Features and Project Goals:**

*   **Open Source and Customizable:** Provides a flexible, modifiable firmware alternative for greater control and customization.
*   **Hardware Support:**
    *   **Volvo OD2 Adapter:**  Based on the STM32F103x8 microcontroller.
    *   **VW NC03 Adapter:** Based on the Nuvoton NUC131 microcontroller.
    *   **VW_NCD01 Adapter:**  Newer version of the VW_NC03, also Nuvoton-based.
    *   **QEMU Emulation:**  Debug and run the STM32F1 version in QEMU, facilitating development without hardware.
*   **MCU Families:**
    *   STMicroelectronics STM32F1 series (STM32F103x8)
    *   Nuvoton NUC131 series
*   **Emulated CANBUS Protocols (for Head Unit Communication):**
    *   Raise VW (PQ)
    *   Raise VW (MQB)
    *   Audi BMW (NBT Evo)
    *   HiWorld VW (MQB)
    *   Mercedes-Benz UDS-based CANBUS (mentioned in the original README)
*   **Confirmed Car Model Support:**
    *   Land Rover Freelander 2 (2007MY, 2013MY)
    *   Volvo XC90 (2007MY)
    *   Volkswagen Tiguan (implied by VW_NC03 adapter)
    *   Skoda Fabia
    *   Audi Q3 2015
    *   Toyota Premio 26x
    *   General support for vehicles using compatible CANBUS adapters
*   **Enhanced Functionality:**
    *   **Data Extraction:** Reads and decodes a wide range of CAN messages from the vehicle's network (often MS-CAN, but adaptable).
    *   **Signal Generation:** Creates necessary analog signals for the Android head unit (IGN/ACC, ILLUM, PARK, REAR) with configurable delays for the reverse camera.
    *   **Protocol Emulation:**  Communicates with the head unit using common protocols, making the adapter appear as a standard unit.
    *   **User Configuration:**  Serial interface (RX/TX) for configuration, debugging, and monitoring.
    *   **Reduced Sleep Mode Power Consumption:** Optimized power usage in sleep mode.
*   **Comprehensive Documentation:**  Includes detailed guides on hardware/software setup, UART debugging, project architecture, and VS Code configuration.
*   **VS Code Integration:**  Pre-configured `launch.json` and `tasks.json` files for debugging with OpenOCD (STM32/Nuvoton) and QEMU (STM32).

## Table of Contents

1.  [Introduction to CANBUS and Android Head Unit Integration](#introduction)
2.  [Hardware and Software Setup](#setup)
    *   [Hardware Requirements](#hardware-requirements)
    *   [Software Prerequisites (Linux)](#software-prerequisites)
    *   [Cloning the Repository](#cloning)
3.  [UART Debugging and Configuration](#uart-debugging)
    *   [Connecting via UART](#uart-connection)
    *   [Entering Debug Mode](#debug-mode)
    *   [Configuration Commands](#configuration-commands)
    *   [Interpreting Debug Output](#debug-output)
    *   [Logging UART Communication](#logging)
4.  [Building and Flashing the Firmware](#building)
    *   [Building for Different Targets](#build-targets)
    *   [Flashing with OpenOCD](#flashing)
    *   [QEMU Emulation and Debugging](#qemu)
5.  [Project Structure and Code Overview](#project-structure)
    *   [Main Components](#main-components)
    *   [Data Flow](#data-flow)
6.  [Adapting to New Vehicles](#new-vehicles)
7. [PlatformIO and VS Code](#platformio)
    * [Installing PlatformIO](#install-platformio)
    * [Building with PlatformIO](#build-platformio)
    * [Debugging with PlatformIO and VS Code](#debug-platformio)
    * [Running QEMU Directly](#qemu-direct)
8.  [Contributing](#contributing)

## 1. <a name="introduction"></a>Introduction to CANBUS and Android Head Unit Integration

A CANBUS (Controller Area Network BUS) is a robust vehicle bus standard that allows microcontrollers and devices to communicate with each other within a vehicle without a host computer.  Many modern vehicles use CANBUS to manage various electronic control units (ECUs), such as the engine control unit, transmission control unit, ABS, airbags, and infotainment systems.

Android head units are aftermarket car stereo systems that run the Android operating system, providing a familiar interface and access to a wide range of apps.  However, these head units often require specific signals and data from the vehicle to function correctly.  This is where a CANBUS adapter comes in.

A CANBUS adapter acts as an intermediary between the vehicle's CAN bus and the Android head unit. It performs the following functions:

*   **Extracts Data:**  Reads messages from the vehicle's CAN bus and decodes them to extract relevant information like ignition status, gear position, speed, steering wheel angle, door status, parking sensor data, and more.
*   **Generates Signals:** Creates the necessary analog signals that many Android head units expect:
    *   **IGN/ACC (Ignition/Accessory):**  Powers the head unit when the ignition is on.
    *   **ILLUM (Illumination):**  Controls backlight brightness.
    *   **PARK (Parking Brake):**  A signal indicating the parking brake is engaged (often used to restrict video playback while driving).
    *   **REAR (Reverse Gear):**  Activates the rearview camera input.
*   **Communicates with Head Unit:**  Sends the extracted and processed vehicle data to the Android head unit over a serial connection (typically UART at 38400 baud).  It emulates a standard CANBUS protocol, making it appear as a compatible adapter to the head unit.

This firmware project replaces the original (often limited) firmware on these adapters, providing greater flexibility, more features, and better control over the interaction between the vehicle and the Android head unit.

## 2. <a name="setup"></a>Hardware and Software Setup

### 2.1. <a name="hardware-requirements"></a>Hardware Requirements

1.  **CANBUS Adapter:**  You'll need a compatible CANBUS adapter.  This project supports the following:
    *   **Volvo OD2 Adapter (STM32F103x8-based):**  Identifiable by "OD-VOLVO-02" markings.
    *   **VW NC03 Adapter (Nuvoton NUC131-based):** Identifiable by "HW:VW-NC-003" markings.
    *   **VW_NCD01 Adapter (Nuvoton-based):** A newer version of the VW_NC03.

2.  **USB-to-Serial Adapter (3.3V):** *Essential* for debugging, configuration, and potentially flashing.  *Do not* use a 5V adapter; it can damage the CAN box.  FTDI FT232R, FT231X, CP210x, or CH340/CH341 chips are recommended.  *Verify the voltage levels with a multimeter before connecting.*

3.  **ST-Link V2 Programmer (for STM32/Nuvoton flashing):** Required to flash the firmware onto the Volvo OD2 and VW NC03 adapters.  *Not* needed for QEMU emulation.

4.  **(Optional) OBD2 Connector:**  Highly recommended for testing and development.  Connects to the vehicle's OBD2 port, providing power, ground, and CAN bus access without needing to tap into the vehicle's wiring harness directly.

5.  **(Optional) Multimeter:** Used to verify voltage levels of the USB-to-serial adapter and to trace connections on the CAN box PCB if needed.

### 2.2. <a name="software-prerequisites"></a>Software Prerequisites (Linux)

These instructions assume a Linux environment (Ubuntu, Fedora, Arch, OpenSUSE, or similar).  A pre-configured virtual machine image is also available (see `doc/sources/DRIVE2.md`).

1.  **ARM Cross-Compiler Toolchain:** Install the `arm-none-eabi-gcc`, `arm-none-eabi-binutils`, and `arm-none-eabi-gdb` packages using your distribution's package manager.  See `doc/env_setup.md` for detailed instructions.
2.  **OpenOCD:** Install OpenOCD (Open On-Chip Debugger) for flashing and debugging on hardware.  Instructions are in `doc/env_setup.md`.  For Nuvoton NUC131, you might need to build OpenOCD from source.
3.  **QEMU (Optional):**  Install `qemu-system-arm` for emulating the STM32F1 version.
4.  **Make:** Install the `make` utility. It is likely already installed.
5.  **Serial Terminal Program:** Install a serial terminal program like `picocom`, `tio`, `minicom`, `screen`, GTKTerm, or CuteCom.  `tio` or `picocom` are recommended for simplicity.
6.  **VS Code and Extensions (Optional):** If you plan to use VS Code, install the "C/C++" and "Cortex-Debug" extensions.
7.  **Git:** To clone the firmware repository.
8. **PlatformIO and Extension (Optional):** If you want to use a full IDE.

Detailed installation commands for various Linux distributions are provided in `doc/env_setup.md`.

### 2.3 <a name="cloning"></a>Cloning the Repository

Clone the firmware repository from GitHub:

```bash
git clone https://github.com/smartgauges/canbox.git
cd canbox-platformio
```

## 3. <a name="uart-debugging"></a>UART Debugging and Configuration

This section explains how to connect to the CAN box, enter debug mode, and interact with the configuration interface.

### 3.1. <a name="uart-connection"></a>Connecting via UART

1.  **Wiring:**  Connect the USB-to-serial adapter to the CAN box's RX/TX lines:

    *   Adapter TX  ->  CAN Box RX
    *   Adapter RX  ->  CAN Box TX
    *   Adapter GND ->  CAN Box GND

    *Do not connect the adapter's VCC pin unless you've verified the voltage.*  Power the CAN box via its normal vehicle connector or OBD2 connector.

2.  **Identify Serial Port:**  Find the device file for the USB-to-serial adapter (e.g., `/dev/ttyUSB0`, `/dev/ttyACM0`).  Use `dmesg | grep tty` or other methods described in `doc/UART.md`.

3.  **Connect with Terminal Program:**  Use a terminal program (e.g., `picocom`, `tio`, `screen`, `minicom`) to connect to the serial port at **38400 baud, 8N1 (8 data bits, no parity, 1 stop bit)**.  Examples:

    *   **picocom:** `picocom -b 38400 /dev/ttyUSB0`
    *   **tio:** `tio -b 38400 /dev/ttyUSB0`
    *   **screen:** `screen /dev/ttyUSB0 38400`
    *   **minicom:** `minicom /dev/ttyUSB0 -b 38400` (configure `minicom -s` first)

### 3.2. <a name="debug-mode"></a>Entering Debug Mode

To enter debug mode, send the character sequence `OOOOOOOOOOOO` (at least 10 'O' characters) quickly (within 1 second) to the CAN box via the serial terminal.  You should see debug output appear in the terminal.

### 3.3. <a name="configuration-commands"></a>Configuration Commands

In debug mode, the following commands are available:

| Command | Description                                                                      |
| :------ | :------------------------------------------------------------------------------- |
| `o`     | Exit debug mode, return to normal operation.                                    |
| `b`     | Select emulated CANBUS protocol (Raise VW(PQ), Raise VW(MQB), Oudi BMW(NBT Evo), HiWorld VW(MQB)). |
| `c`     | Select car model (FL2 2007MY, FL2 2013MY, XC90 2007MY).                            |
| `I`     | Increase backlight level threshold.                                            |
| `i`     | Decrease backlight level threshold.                                            |
| `D`     | Increase rear camera turn-off delay.                                           |
| `d`     | Decrease rear camera turn-off delay.                                           |
| `m`     | Select active CAN message for display.                                          |
| `s`     | Save configuration to non-volatile memory.                                     |
|`S`      | Switch to CAN sniffer mode.                                                    |

### 3.4. <a name="debug-output"></a>Interpreting the Debug Output

The debug output provides valuable information about the CAN box's operation and the vehicle's CAN bus.  Here's an example and explanation:

```
Main window
--------------------
Configuration(63)
--------------------
Car: Any Msg Vin: na  0 km
CanBox: Raise VW(PQ)
Conf: Illum:50 Rear Delay:1500
State
--------------------
Uptime: 00003.770 Wakeups: 0 Wheel:0
Acc:1 Ign:1 Selector:x R:0
Illum:0 ParkLights:0 NearLights:0
Park:ff Front:00 00 00 00 Rear:00 00 00 00
Can: IDs:3 Msgs:18 Irqs:0
Can1/3:  0000007e:00.00.98.07.d0.00.80.00
--------------------
Ctrl keys: o - exit; m - shift msgs
I/i - set Illum; d/D - set rear Delay
c - set Car; b - set Canbox
s - save conf; S - to Sniffer window
```

*   **Configuration:**  Displays current settings (car model, CAN box type, illumination threshold, rear camera delay).
*   **State:** Shows the current status of the CAN box and decoded vehicle data (uptime, ignition, gear selector, lights, parking sensors, etc.).
*   **Can:** Shows statistics about received CAN messages (unique IDs, total messages, interrupts).
*   **CanX/Y:** Displays the raw data of a specific CAN message (ID and data bytes).  Use the `m` command to cycle through received messages.
*   **Ctrl keys:** Lists the available commands in debug mode.

### 3.5. <a name="logging"></a>Logging UART Communication

To capture all communication between the CAN box and the Android head unit (or your debugging setup) to a file, you can use shell redirection:

```bash
picocom -b 38400 /dev/ttyUSB0 > can_log.txt 2>&1 &
```

Or, with `tio`:
```bash
tio -b 38400 /dev/ttyUSB0 > can_log.txt 2>&1 &
```

This command:

*   Starts `picocom` or `tio` in the background (`&`).
*   Redirects standard output (`>`) to `can_log.txt`.
*   Redirects standard error (`2>&1`) to the same file as standard output.

This will capture *all* serial communication, including your debug commands and the CAN box's responses.

**Using `minicom`'s Capture Feature:**

1.  Press `Ctrl+A`, then `L` to start capture.
2.  Enter a filename (e.g., `canbox_log.txt`).
3.  Connect to the CAN box: `minicom /dev/ttyUSB0 -b 38400`.
4.  Enter debug mode and interact with the system.
5.  Press `Ctrl+A`, then `L` again to stop capture.
6.  Exit `minicom` (`Ctrl+A`, then `X`).

## 4. <a name="building"></a>Building and Flashing the Firmware

### 4.1. <a name="build-targets"></a>Build Targets

The project uses Makefiles for building.  The main Makefile supports these targets:

*   `make`: Builds all targets (firmware for VW NC03, Volvo OD2, and QEMU).
*   `make vw_nc03.bin`: Builds firmware for the VW NC03 adapter.
*   `make volvo_od2.bin`: Builds firmware for the Volvo OD2 adapter.
*   `make qemu.bin`: Builds firmware for QEMU emulation.

### 4.2. <a name="flashing"></a>Flashing with OpenOCD

Use the following `make` commands with an ST-Link V2 programmer:

*   `make flash_vw_nc03`: Flashes the `vw_nc03.bin` firmware to a Raise VW NC03 adapter.
*   `make flash_volvo_od2`: Flashes the `volvo_od2.bin` firmware to a Raise Volvo OD2 adapter.

**Important Notes for Nuvoton NUC131 (VW NC03):**

*   You may need a specific version of OpenOCD (potentially built from source) for the Nuvoton NUC131. See `doc/env_setup.md` and `doc/sources/DRIVE2.md` for details.
*   You will likely need to create a `target/NUC131.cfg` file for OpenOCD.
*   ST-Link connection: Connect to the internal CON2 connector on the VW NC03 board.  Use the schematic or a multimeter to determine the correct pinout (GND, PWR, SWDIO, SWCLK, RST).

### 4.3. <a name="qemu"></a>QEMU Emulation and Debugging

1.  **Build for QEMU:** `make qemu`
2.  **Run in QEMU:** `make run_qemu`

This will compile the `qemu.bin` file and run QEMU to simulate the STM32F1 microcontroller.

**Debugging with GDB (within QEMU):**

1.  Start QEMU in debug mode (with `-S -s` options) in one terminal:

    ```bash
    make run_qemu
    ```

    Or, if you want to run it manually:

    ```bash
    qemu-system-arm -M stm32-p103 -kernel firmware.elf -nographic -S -s
    ```

2.  Start a GDB session in *another* terminal:

    ```bash
    arm-none-eabi-gdb firmware.elf
    ```

3.  Connect GDB to QEMU:

    ```
    target remote :1234
    ```

4.  You can now set breakpoints, step through code, inspect variables, etc., within the GDB session.

## 5. <a name="project-structure"></a>Project Structure and Code Overview

The firmware is designed to be modular, making it easier to add support for new vehicles and CANBUS protocols.  Here's a breakdown of the key components:

### 5.1. <a name="main-components"></a>Main Components

*   **`main.c`:**  The main application loop.  Initializes hardware, handles the main processing loop, and calls functions from other modules.  Includes debug output and command handling.

*   **`hw.c`, `hw.h`:**  Hardware Abstraction Layer (HAL).  Provides a consistent interface to the microcontroller's peripherals (GPIO, USART, CAN, timers, etc.), regardless of the specific MCU (STM32F1 or NUC131).  Platform-specific code is isolated here.

*   **`hw_*.c`, `hw_*.h`:**  Individual HAL implementations for specific peripherals (e.g., `hw_can.c`, `hw_usart.c`, `hw_tick.c`, `hw_gpio.c`).

*   **`canbox.c`, `canbox.h`:**  Implements communication with the Android head unit over the serial interface (RX/TX).  Handles the emulated CANBUS protocols (Raise, HiWorld, etc.), sending vehicle data and processing head unit commands.

*   **`car.c`, `car.h`:**  Contains the *vehicle-specific* logic for decoding CAN bus messages.  This is the core component for adapting the firmware to different car models.  It provides a standardized interface for accessing vehicle data (e.g., `car_get_speed()`, `car_get_door_fl()`).

*   **`conf.c`, `conf.h`:**  Manages reading and writing configuration data (e.g., selected vehicle, CANBUS protocol, delays) to non-volatile storage (flash memory). `hw_conf.h` provides platform-specific details for accessing the storage.

*   **`ring.c`, `ring.h`:**  Implements a ring buffer (circular buffer) for efficient handling of serial data.

### Subdirectories

*   **`cars/`:**  *Crucially*, this directory contains vehicle-specific CAN message handling. Each car model has its own `.c` file (e.g., `lr2_2007my.c`, `xc90_2007my.c`). These files:
    *   Define `msg_desc_t` structures: These describe the CAN messages to be monitored (ID, period, handler function).
    *   Implement handler functions: These decode the data from received CAN messages and update the `carstate` structure.

*   **`qemu/`, `volvo_od2/`, `vw_nc03/`:**  Platform-specific files (linker scripts, hardware initialization) for QEMU, Volvo OD2, and VW NC03.

### 5.2. <a name="data-flow"></a>Data Flow

1.  **CAN Message Reception:** The `hw_can` module receives CAN messages.
2.  **Message Filtering and Handling:** `car_process()` (in `car.c`) is called periodically. It retrieves received messages and uses the `msg_desc_t` structures (in the car-specific files) to identify and process relevant messages.
3.  **Vehicle State Update:**  CAN message handler functions (e.g., `lr2_2007my_ms_6c_handler`) decode the data and update the `carstate` structure.
4.  **Signal Generation:** The `main()` function calls `gpio_process()`, which uses `carstate` to control output signals (IGN, ILLUM, REAR, PARK).
5.  **Head Unit Communication:** `canbox_process()` (in `canbox.c`) handles serial communication with the head unit, sending vehicle data and receiving commands.
6.  **Configuration:** `conf_read()` and `conf_write()` manage persistent configuration.
7.  **Debug Mode:** `debug_ch_process()` handles serial commands in debug mode.

## 6. <a name="new-vehicles"></a>Adapting to New Vehicles

The most critical step in adapting this firmware to a new vehicle is to **reverse engineer the relevant CAN bus messages** for that specific car model and year.  This often involves:

1.  **Identifying the CAN Bus:**  Determine which CAN bus (e.g., MS-CAN, HS-CAN) contains the necessary data.  Vehicle wiring diagrams and online resources are helpful.
2.  **CAN Bus Sniffing:**  Use a CAN bus analyzer (such as the setup described with a USB-to-serial adapter and `picocom` or `tio` in sniffer mode) to capture raw CAN messages while performing specific actions in the car (e.g., turning on the ignition, opening doors, shifting gears).
3.  **Message Identification:**  Analyze the captured CAN data to identify the message IDs and data bytes that correspond to the desired vehicle parameters.  Look for patterns and changes in the data that correlate with the actions you performed. The `plot` program mentioned in `DRIVE2.md` can be helpful for visualizing this data.
4.  **Creating a Car-Specific File:**  Create a new `.c` file in the `cars/` directory (e.g., `my_car_model.c`).  Define the `msg_desc_t` structures and handler functions to decode the relevant CAN messages and update the `carstate` structure.
5.  **Updating `conf.c` and `car.c`:**  Add support for the new car model in `conf.c` (to store the selected car) and `car.c` (to initialize and process data for the new model).
6.  **Testing and Refinement:**  Thoroughly test the implementation and refine the CAN message decoding as needed.

## <a name="platformio"></a>7. PlatformIO and VS Code

This project is designed to be built and managed using PlatformIO, a cross-platform, open-source ecosystem for IoT development. PlatformIO provides a unified build system, library manager, and debugging tools, simplifying the development process.  The recommended IDE is Visual Studio Code (VS Code) with the PlatformIO extension.

### 7.1. <a name="install-platformio"></a>Installing PlatformIO

1.  **Install VS Code:** Download and install VS Code from [https://code.visualstudio.com/](https://code.visualstudio.com/).
2.  **Install the PlatformIO IDE Extension:** Open VS Code, go to the Extensions view (Ctrl+Shift+X or Cmd+Shift+X), search for "PlatformIO IDE", and click "Install". Restart VS Code when prompted.

### 7.2. <a name="build-platformio"></a>Building with PlatformIO

The `platformio.ini` file in the project root configures the build process for different targets.

1.  **Open the Project:** In VS Code, open the `canbox-platformio` folder (the one containing `platformio.ini`). PlatformIO should automatically detect the project.
2.  **Build:**
    *   Click the "PlatformIO" icon in the activity bar (the alien head).
    *   In the "Project Tasks" section, expand the environment you want to build (e.g., `env:genericSTM32F103C8`, `env:qemu`).
    *   Click "Build".

    Alternatively, you can use the PlatformIO CLI:

    *   Open a terminal in the project directory.
    *   Run `pio run -e <environment>` (e.g., `pio run -e genericSTM32F103C8` or `pio run -e qemu`).

The compiled firmware (`firmware.elf` and `firmware.bin`) will be located in the `.pio/build/<environment>` directory.

### 7.3. <a name="debug-platformio"></a>Debugging with PlatformIO and VS Code

The `.vscode/launch.json` and `.vscode/tasks.json` files provide pre-configured debugging setups for both on-hardware debugging (using OpenOCD and an ST-Link V2) and QEMU emulation.

1.  **Prerequisites:** Ensure you have the necessary tools installed (ARM toolchain, OpenOCD, QEMU) as described in the "Software Prerequisites" section.
2.  **Connect Hardware (if applicable):** For on-hardware debugging (STM32 or Nuvoton), connect your ST-Link V2 programmer to the target board.
3.  **Select Debug Configuration:** In VS Code, go to the "Run and Debug" view (Ctrl+Shift+D or Cmd+Shift+D). Select the desired configuration from the dropdown menu (e.g., "OpenOCD (STM32 - Volvo OD2)", "OpenOCD (Nuvoton NUC131 - VW NC03)", or "QEMU (STM32)").
4.  **Start Debugging:** Press F5 or click the "Start Debugging" button. The `preLaunchTask` in `launch.json` will automatically build the appropriate firmware target before starting the debugger.
5. **Debug tasks**: The debugging tasks in `.vscode/tasks.json` can be also launched from "Terminal"->"Run Task..."

**Troubleshooting:**

*   If you encounter issues with OpenOCD, make sure it's correctly installed and configured (especially for the Nuvoton NUC131).  You may need to create a custom `target/NUC131.cfg` file.
*   Ensure the correct GDB executable (`arm-none-eabi-gdb`) is specified in `launch.json` and is in your system's PATH.
*   Verify that the `configFiles` in `launch.json` point to the correct interface and target configuration files for your hardware.

### 7.4. <a name="qemu-direct"></a>Running QEMU Directly

For more advanced QEMU usage or troubleshooting, you can run QEMU directly from the command line:

1.  **Build the QEMU target:**  `make qemu` (or use the PlatformIO build task).

2.  **Run QEMU:**

    ```bash
    qemu-system-arm -M stm32-p103 -kernel .pio/build/qemu/firmware.elf -serial stdio -display none
    ```

    This command:

    *   `qemu-system-arm`: Starts the ARM system emulator.
    *   `-M stm32-p103`: Specifies the emulated machine (STM32-P103 board).
    *   `-kernel .pio/build/qemu/firmware.elf`:  Loads the compiled ELF firmware.
    *   `-serial stdio`: Redirects the serial port (USART) to the standard input/output, allowing you to interact with the debug interface.
    *   `-display none`:  Runs QEMU without a graphical display (headless mode).

    You can then use a serial terminal program (like `picocom` or `tio`) to connect to the emulated serial port.

    For debugging with GDB, add the `-S -s` options:

    ```bash
    qemu-system-arm -M stm32-p103 -kernel .pio/build/qemu/firmware.elf -serial stdio -display none -S -s
    ```

    *   `-S`:  Halts the CPU at startup.
    *   `-s`:  Creates a GDB server on port 1234.

    Then, connect GDB as described in the "QEMU Emulation and Debugging" section above.

## 8. <a name="contributing"></a>Contributing

Contributions to this project are welcome!  Here's how you can contribute:

*   **Add Support for New Vehicles:**  Reverse engineer the CAN messages for your car and create a new car-specific file in the `cars/` directory.
*   **Improve Existing Vehicle Support:**  Refine the CAN message decoding or add support for additional features.
*   **Enhance the Firmware:**  Add new functionality, improve code efficiency, or fix bugs.
*   **Improve Documentation:**  Update and expand the documentation to make it more comprehensive and user-friendly.
*   **Report Issues:**  If you encounter problems or have suggestions, open an issue on the GitHub repository.
*   **Submit Pull Requests:**  If you've made changes, submit a pull request to have your contributions reviewed and merged.

When contributing, please follow these guidelines:

*   **Code Style:**  Maintain a consistent coding style with the existing code.
*   **Documentation:**  Update the documentation (including the README and code comments) to reflect your changes.
*   **Testing:**  Thoroughly test your changes, ideally on real hardware