cmake_minimum_required(VERSION 3.13)
set(PICO_PLATFORM rp2350-arm-s)
set(PICO_BOARD pico2)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 26)
set(PICO_CXX_ENABLE_EXCEPTIONS 1)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

# Pull in FreeRTOS
set(FREERTOS_KERNEL_PATH ${CMAKE_CURRENT_LIST_DIR}/freertos)
include(FreeRTOS_Kernel_import.cmake)

# Project name
project(KrakenMachineFirmware)


pico_sdk_init()

# Configuration projects
add_executable(KrakenMachineFirmware
    src/main.c
)

target_include_directories(KrakenMachineFirmware PRIVATE
    include/
)

target_link_libraries(KrakenMachineFirmware PRIVATE
    pico_stdlib
    FreeRTOS-Kernel
    FreeRTOS-Kernel-Heap4
)

# enable usb output, disable uart output
pico_enable_stdio_usb(KrakenMachineFirmware 1)
pico_enable_stdio_uart(KrakenMachineFirmware 0)

target_compile_options(KrakenMachineFirmware PRIVATE -Wall)

pico_add_extra_outputs(KrakenMachineFirmware)
