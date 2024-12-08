cmake_minimum_required(VERSION 3.17)

# Set platform and board
set(PICO_PLATFORM rp2350-arm-s)
set(PICO_BOARD pico2)

# Set C and C++ standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(PICO_CXX_ENABLE_EXCEPTIONS 1)

include(../pico_sdk_import.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/../tinyusb/hw/bsp/family_support.cmake)
include(../FreeRTOS_Kernel_import.cmake)

#family_get_project_name(PROJECT ${CMAKE_CURRENT_LIST_DIR})


# Set project name
project(KrakenMachineFirmware C CXX ASM)

# Initialize Pico SDK
pico_sdk_init()
family_initialize_project(KrakenMachineFirmware ${CMAKE_CURRENT_LIST_DIR})

# Add main executable for Kraken Machine Firmware
add_executable(KrakenMachineFirmware
    main.c
    usb/msc_disk.c
    usb/usb_descriptors.c
)


# Add KrakenMachineScheduler library
add_library(KrakenMachineScheduler
    KrakenMachineScheduler/kraken_scheduler.c
    KrakenMachineScheduler/kraken_hal.c
)
target_include_directories(KrakenMachineScheduler PUBLIC
    ../include
    KrakenMachineScheduler
)
target_link_libraries(KrakenMachineScheduler PUBLIC
    pico_stdlib
    pico_multicore
    FreeRTOS-Kernel
    FreeRTOS-Kernel-Heap4
)


# Configure TinyUSB and board support for the project
family_configure_device_example(KrakenMachineFirmware noos)


target_include_directories(KrakenMachineFirmware PUBLIC
    KrakenMachineScheduler
#    ${CMAKE_CURRENT_SOURCE_DIR}/../tinyusb
#    ${CMAKE_CURRENT_SOURCE_DIR}/../include
#    ${CMAKE_CURRENT_SOURCE_DIR}/../tinyusb/src
)

# Link required libraries for KrakenMachineFirmware
target_link_libraries(KrakenMachineFirmware PUBLIC
    pico_stdlib
    pico_multicore
    KrakenMachineScheduler
#    FreeRTOS-Kernel
#    FreeRTOS-Kernel-Heap4
#    tinyusb_device
#    tinyusb_additions
#    tinyusb_board
    hardware_gpio
    hardware_flash
    hardware_uart
    hardware_spi
)

# Add extra outputs (e.g., UF2 binary)
pico_add_extra_outputs(KrakenMachineFirmware)

# Debugging information
message(STATUS "Configured Kraken Machine Firmware with TinyUSB and FreeRTOS.")
