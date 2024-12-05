cmake_minimum_required(VERSION 3.13)

# Set platform and board
set(PICO_PLATFORM rp2350-arm-s)
set(PICO_BOARD pico2)

# Set C and C++ standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(PICO_CXX_ENABLE_EXCEPTIONS 1)

# Include Pico SDK
include(pico_sdk_import.cmake)

# Include FreeRTOS Kernel
set(FREERTOS_KERNEL_PATH ${CMAKE_CURRENT_LIST_DIR}/freertos)
include(FreeRTOS_Kernel_import.cmake)

project(KrakenMachineFirmware)
pico_sdk_init()


# Add KrakenMachineScheduler library
add_library(KrakenMachineScheduler
    src/KrakenMachineScheduler/kraken_scheduler.c
    src/KrakenMachineScheduler/kraken_hal.c
)
target_include_directories(KrakenMachineScheduler PRIVATE
    include/
    src/KrakenMachineScheduler
)
target_link_libraries(KrakenMachineScheduler
    pico_stdlib
    pico_multicore
    FreeRTOS-Kernel
    FreeRTOS-Kernel-Heap4
)

# Add KrakenMachineFirmware executable
add_executable(KrakenMachineFirmware
    src/main.c
)
target_include_directories(KrakenMachineFirmware PRIVATE
    include/
    src/KrakenMachineScheduler
)


target_link_libraries(KrakenMachineFirmware
    pico_stdlib
    KrakenMachineScheduler
)

# Enable USB and disable UART for stdio
pico_enable_stdio_usb(KrakenMachineFirmware 1)
pico_enable_stdio_uart(KrakenMachineFirmware 0)

# Add extra outputs (e.g., UF2, BIN)
pico_add_extra_outputs(KrakenMachineFirmware)
