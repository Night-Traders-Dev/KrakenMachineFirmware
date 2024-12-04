Here’s a detailed and professional `README.md` file for your Kraken Machine Firmware GitHub repository:

```markdown
# Kraken Machine Firmware

The **Kraken Machine Firmware** is a modular and extensible firmware designed for the Kraken Machine, a custom hardware platform inspired by devices like the Flipper Zero. The firmware integrates the **FreeRTOS** real-time operating system and a custom scheduler (`KrakenMachineScheduler`) to manage tasks, hardware abstractions, and advanced functionalities.

## Features

- **Real-Time Task Scheduling**:
  - Powered by FreeRTOS for efficient multitasking.
  - Custom `KrakenMachineScheduler` for task management.
- **Hardware Abstraction Layer**:
  - Simplifies interactions with GPIO, peripherals, and onboard components.
- **Support for Raspberry Pi Pico and RP2350**:
  - Built for high performance and scalability.
- **USB Serial Debugging**:
  - View logs and outputs via a USB connection.
- **Modular Codebase**:
  - Separate layers for core scheduling, hardware abstraction, and application logic.

## Directory Structure

```
KrakenMachineFirmware/
├── CMakeLists.txt                # Main CMake build configuration
├── FreeRTOS_Kernel_import.cmake  # FreeRTOS integration
├── freertos/                     # FreeRTOS kernel source
├── include/                      # Shared header files
├── pico_sdk_import.cmake         # Pico SDK integration
├── src/                          # Source code
│   ├── KrakenMachineScheduler/   # Custom scheduler implementation
│   │   ├── kraken_scheduler.c
│   │   └── kraken_scheduler.h
│   └── main.c                    # Main application entry point
└── README.md                     # Project documentation
```

## Getting Started

### Prerequisites

To build and flash the firmware, you will need:

- **Raspberry Pi Pico SDK** (installed at `/opt/pico-sdk` or update `CMakeLists.txt` to match your setup)
- **FreeRTOS Kernel** (included in this repository under `freertos/`)
- **CMake** (version 3.13 or newer)
- **GNU ARM Embedded Toolchain** (e.g., `arm-none-eabi-gcc`)

### Building the Firmware

1. Clone the repository:
   ```bash
   git clone https://github.com/Night-Traders-Dev/KrakenMachineFirmware.git
   cd KrakenMachineFirmware
   git submodule update --init --jobs 24 
   ```

2. Create a build directory and configure the project:
   ```bash
   mkdir build
   cd build
   cmake ..
   ```

3. Compile the firmware:
   ```bash
   make -j$(nproc)
   ```

4. After a successful build, the firmware will be available as a `.uf2` file.

### Flashing the Firmware

1. Connect the Kraken Machine to your computer via USB.
2. Put the device in BOOTSEL mode by holding down the BOOTSEL button while plugging in the USB cable.
3. Drag and drop the generated `.uf2` file onto the mounted storage device.

### Viewing Debug Output

To view debug messages and logs:

1. Open a terminal application (e.g., `screen`, `minicom`, PuTTY).
2. Connect to the serial port with a baud rate of **115200**.
   ```bash
   screen /dev/ttyACM0 115200
   ```

## Development

### Adding Features

1. **Custom Tasks**:
   Add new tasks to `main.c` or integrate them via `KrakenMachineScheduler` for better abstraction.

2. **Hardware Drivers**:
   Implement additional hardware support in `kraken_hal.c` and `kraken_hal.h`.

3. **Modules**:
   Organize feature-specific code into new directories under `src/`.

### Debugging

- Use the USB serial output to monitor logs and task behavior.
- Enable debugging symbols in `CMakeLists.txt`:
  ```cmake
  set(CMAKE_BUILD_TYPE Debug)
  target_compile_options(KrakenMachineFirmware PRIVATE -g)
  ```

## Contributing

We welcome contributions to improve the Kraken Machine Firmware. To contribute:

1. Fork the repository.
2. Create a feature branch:
   ```bash
   git checkout -b feature/your-feature-name
   ```
3. Commit your changes and submit a pull request.

## License

This project is licensed under the [MIT License](LICENSE).

## Acknowledgments

- **FreeRTOS**: Real-time operating system powering the firmware.
- **Raspberry Pi Pico SDK**: Base framework for RP2040 and RP2350 microcontrollers.
- **Flipper Zero**: Inspiration for the Kraken Machine hardware and firmware architecture.

---
