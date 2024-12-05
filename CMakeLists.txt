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

project(KrakenMachineFirmware C CXX ASM)

# Initialize Pico SDK
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


add_executable(KrakenMachineFirmware
    src/main.c
    src/usb/usb_msc.c
    src/fs/flash.c
    src/fs/fatfs_driver.c
)


target_include_directories(KrakenMachineFirmware PRIVATE
    include/
    src/KrakenMachineScheduler
    src/usb
    src/fs
    src/io
    ${PICO_SDK_PATH}/lib/tinyusb/lib/fatfs/source
    ${PICO_SDK_PATH}/lib/tinyusb/src
    ${PICO_SDK_PATH}/lib/tinyusb/src/device
    ${PICO_SDK_PATH}/lib/tinyusb/src/class/msc

)


# Link required libraries
target_link_libraries(KrakenMachineFirmware
    pico_stdlib
    pico_multicore
    tinyusb_additions
    tinyusb_board
    tinyusb_device
    hardware_flash
    hardware_sync
    KrakenMachineScheduler 
)


# Enable USB and disable UART for stdio
pico_enable_stdio_usb(KrakenMachineFirmware 1)
pico_enable_stdio_uart(KrakenMachineFirmware 0)

pico_add_extra_outputs(KrakenMachineFirmware)

