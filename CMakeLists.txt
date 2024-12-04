cmake_minimum_required(VERSION 3.13)

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

target_compile_options(KrakenMachineFirmware PRIVATE -Wall)

pico_add_extra_outputs(KrakenMachineFirmware)
