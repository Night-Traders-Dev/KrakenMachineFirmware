#ifndef KRAKEN_SCHEDULER_H
#define KRAKEN_SCHEDULER_H

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

// Task management
typedef struct {
    TaskHandle_t handle;
    const char* name;
    UBaseType_t priority;
    uint16_t stackSize;
} KrakenTask;

// Task creation
KrakenTask* kraken_create_task(void (*taskFunction)(void*), const char* name, uint16_t stackSize, void* params, UBaseType_t priority);

// IPC mechanisms
QueueHandle_t kraken_create_queue(UBaseType_t queueLength, UBaseType_t itemSize);
BaseType_t kraken_send_message(QueueHandle_t queue, void* message, TickType_t timeout);
BaseType_t kraken_receive_message(QueueHandle_t queue, void* buffer, TickType_t timeout);

// Timer utilities
TimerHandle_t kraken_create_timer(const char* name, TickType_t period, BaseType_t autoReload, void* id, TimerCallbackFunction_t callback);

#endif // KRAKEN_SCHEDULER_H
