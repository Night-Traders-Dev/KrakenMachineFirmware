cmake_minimum_required(VERSION 3.13)

# Set platform and board
set(PICO_PLATFORM rp2350-arm-s)
set(PICO_BOARD pico2)

# Set C and C++ standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(PICO_CXX_ENABLE_EXCEPTIONS 1)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

# Pull in FreeRTOS
set(FREERTOS_KERNEL_PATH ${CMAKE_CURRENT_LIST_DIR}/freertos)
include(FreeRTOS_Kernel_import.cmake)

project(KrakenMachineFirmware)

pico_sdk_init()

add_executable(KrakenMachineFirmware
    src/main.c
)

target_include_directories(KrakenMachineFirmware PRIVATE
    include/
)

target_include_directories(KrakenMachineFirmware PUBLIC src/KrakenMachineScheduler)


add_library(KrakenMachineScheduler
    src/KrakenMachineScheduler/kraken_scheduler.c
)

target_include_directories(KrakenMachineScheduler PRIVATE
    include/
)

target_link_libraries(KrakenMachineFirmware
    pico_stdlib
#    FreeRTOS-Kernel
#    FreeRTOS-Kernel-Heap4
    KrakenMachineScheduler
)

target_link_libraries(KrakenMachineScheduler
    pico_stdlib
    FreeRTOS-Kernel
    FreeRTOS-Kernel-Heap4
)

pico_enable_stdio_usb(KrakenMachineFirmware 1)
pico_enable_stdio_uart(KrakenMachineFirmware 0)

set(CMAKE_BUILD_TYPE Debug)
target_compile_options(KrakenMachineFirmware PRIVATE -Wall -g)

pico_add_extra_outputs(KrakenMachineFirmware)
