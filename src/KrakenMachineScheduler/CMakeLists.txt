project(KrakenMachineScheduler)

add_library(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} ${PROJECT_SOURCE_DIR})

target_sources(${PROJECT_NAME}
    PRIVATE
        KrakenMachineScheduler/kraken_scheduler.c
        KrakenMachineScheduler/kraken_scheduler.h
        KrakenMachineScheduler/kraken_hal.c
        KrakenMachineScheduler/kraken_hal.h
)
