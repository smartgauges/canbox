# Environment Setup for CANBUS Adapter Firmware Development

This document outlines the necessary software and packages to set up your development environment for compiling, debugging, and flashing the CANBUS adapter firmware. These instructions are provided for various Linux distributions, including Ubuntu, Fedora, Arch Linux, and **OpenSUSE**. Adapt these instructions for other operating systems as needed.

## 1. Install ARM Cross-Compiler Toolchain

You'll need an ARM cross-compiler toolchain to compile code for the ARM Cortex-M microcontrollers used in the CANBUS adapters (STM32F1 and Nuvoton NUC131). We will use `arm-none-eabi-gcc`.

**Ubuntu/Debian:**

```bash
sudo apt update
sudo apt install arm-none-eabi-gcc arm-none-eabi-binutils arm-none-eabi-gdb
```

**Fedora/CentOS/RHEL:**

```bash
sudo dnf install arm-none-eabi-gcc arm-none-eabi-binutils arm-none-eabi-gdb
```

**Arch Linux/Manjaro:**

```bash
sudo pacman -S arm-none-eabi-gcc arm-none-eabi-binutils arm-none-eabi-gdb
```

**OpenSUSE:**

```bash
sudo zypper install cross-arm-none-gcc12 cross-arm-binutils cross-arm-gcc12 cross-arm-linux-glibc-devel cross-arm-none-newlib-devel
```

**Other Distributions:**

Use your distribution's package manager to search for and install the `arm-none-eabi-gcc`, `arm-none-eabi-binutils`, and `arm-none-eabi-gdb` packages.

**Verification:**

After installation, verify that the compiler is correctly installed and in your system's PATH by running:

```bash
arm-none-eabi-gcc -v
```

You should see version information for the ARM GCC compiler.

## 2. Install OpenOCD

OpenOCD (Open On-Chip Debugger) is required for flashing the firmware to the CANBUS adapter hardware using an ST-Link V2 programmer and for debugging on hardware.

**Ubuntu/Debian:**

```bash
sudo apt update
sudo apt install openocd
```

**Fedora/CentOS/RHEL:**

```bash
sudo dnf install openocd
```

**Arch Linux/Manjaro:**

```bash
sudo pacman -S openocd
```

**OpenSUSE:**

```bash
sudo zypper install openocd
```

**For Nuvoton NUC131 (VW_NC03):**

For some Nuvoton NUC131 adapters, you might need a specific version of OpenOCD. It's recommended to build it from source:

```bash
sudo apt update
sudo apt install build-essential libtool pkg-config autoconf automake texinfo libusb-1.0-0-dev

git clone https://github.com/OpenNuvoton/OpenOCD-Nuvoton.git
cd OpenOCD-Nuvoton
./bootstrap
./configure --disable-werror --disable-shared --enable-ftdi
make
sudo make install
```

**Verification:**

Verify OpenOCD installation:

```bash
openocd --version
```

## 3. Install QEMU (Optional, for Emulation)

QEMU is an emulator that allows you to run and debug the firmware in a simulated STM32F103 environment without needing the physical hardware. It's useful for initial development and testing.

**Ubuntu/Debian:**

```bash
sudo apt update
sudo apt install qemu-system-arm
```

**Fedora/CentOS/RHEL:**

```bash
sudo dnf install qemu-system-arm
```

**Arch Linux/Manjaro:**

```bash
sudo pacman -S qemu-system-arm
```

**OpenSUSE:**

```bash
sudo zypper install qemu-arm
```

**Verification:**

```bash
qemu-system-arm --version
```

## 4. Install Make

Make is a build automation tool used to manage the firmware compilation process using the provided Makefiles. It's likely already installed on most Linux systems.

**Ubuntu/Debian:**

```bash
sudo apt update
sudo apt install make
```

**Fedora/CentOS/RHEL:**

```bash
sudo dnf install make
```

**Arch Linux/Manjaro:**

```bash
sudo pacman -S make
```

**OpenSUSE:**

```bash
sudo zypper install make
```

**Verification:**

```bash
make --version
```

## 5. Install a Serial Terminal Program (for Debugging)

A serial terminal program is needed to communicate with the CANBUS adapter's UART interface for debugging and configuration. Some recommended options are:

*   **`picocom` (lightweight and simple):**

    ```bash
    sudo apt update
    sudo apt install picocom
    ```

*   **`tio` (modern and user-friendly):**
    ```bash
    sudo apt update
    sudo apt install tio
    ```
*   **`minicom` (feature-rich, text-based):**

    ```bash
    sudo apt update
    sudo apt install minicom
    ```

*   **`screen` (basic, often pre-installed):**
     Usually pre-installed, no need to install

    **OpenSUSE:**
    For all terminal programs:

    ```bash
    sudo zypper install picocom tio minicom screen
    ```
    *(You can choose to install only one or more of these)*

## 6. Install VS Code and Extensions (Optional, for IDE)

VS Code (Visual Studio Code) is a popular code editor that can be configured for C/C++ development, debugging, and flashing.

1.  **Install VS Code:** Download and install VS Code from the official website: [https://code.visualstudio.com/](https://code.visualstudio.com/)

2.  **Install VS Code Extensions:** Open VS Code and install the following extensions from the Extensions marketplace (Ctrl+Shift+X or Cmd+Shift+X):
    *   **C/C++ (ms-vscode.cpptools):** Provides C/C++ language support.
    *   **Cortex-Debug (marus25.cortex-debug):** Provides debugging support for ARM Cortex-M microcontrollers.

## 7. Set up User Permissions (Linux - for Serial Port Access)

To access the serial port without `sudo`, add your user to the `dialout` group:

**Ubuntu/Debian, Fedora, Arch Linux, OpenSUSE:**

```bash
sudo usermod -a -G dialout $USER
```

Log out and log back in for the group changes to take effect.

## 8. Clone the Firmware Repository

Clone the CANBUS adapter firmware repository from GitHub:

```bash
git clone <repository_url>  # Replace <repository_url> with the actual repository URL
cd <repository_directory> # Go to the cloned repository directory
```

## 9. Install Git (If Not Already Installed)

Git is required to clone the firmware repository. It's likely already installed.

**Ubuntu/Debian:**

```bash
sudo apt update
sudo apt install git
```

**Fedora/CentOS/RHEL:**

```bash
sudo dnf install git
```

**Arch Linux/Manjaro:**

```bash
sudo pacman -S git
```

**OpenSUSE:**

```bash
sudo zypper install git
```
## 10. Install a virtual machine (Optional, if you want to use pre-configured environment)
Download and install VirtualBox or VMware, and then download and import the virtual machine image provided in the `README.md` or `DRIVE2.md` documentation.

## Summary

After completing these steps, you should have a fully functional development environment set up to:

*   **Compile:** Build the CANBUS adapter firmware for different targets (VW_NC03, Volvo OD2, QEMU, Peugeot 407).
*   **Debug:** Debug the firmware using QEMU or on actual hardware with OpenOCD and VS Code/GDB.
*   **Flash:** Program the firmware to the CANBUS adapter hardware using an ST-Link V2 programmer and OpenOCD.
*   **Monitor and Configure:** Communicate with the CANBUS adapter via a serial terminal for debugging, configuration, and CAN bus sniffing.

This comprehensive environment setup will enable you to effectively develop, test, and customize the CANBUS adapter firmware for your specific vehicle and Android head unit integration project.

For detailed environment setup instructions, please refer to `env_setup.md`.
