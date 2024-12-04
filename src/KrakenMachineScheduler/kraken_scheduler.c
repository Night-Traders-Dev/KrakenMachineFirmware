#include "kraken_scheduler.h"

KrakenTask* kraken_create_task(void (*taskFunction)(void*), const char* name, uint16_t stackSize, void* params, UBaseType_t priority) {
    KrakenTask* task = pvPortMalloc(sizeof(KrakenTask));
    if (task == NULL) return NULL;

    task->name = name;
    task->priority = priority;
    task->stackSize = stackSize;

    if (xTaskCreate(taskFunction, name, stackSize, params, priority, &task->handle) != pdPASS) {
        vPortFree(task);
        return NULL;
    }

    return task;
}

QueueHandle_t kraken_create_queue(UBaseType_t queueLength, UBaseType_t itemSize) {
    return xQueueCreate(queueLength, itemSize);
}

BaseType_t kraken_send_message(QueueHandle_t queue, void* message, TickType_t timeout) {
    return xQueueSend(queue, message, timeout);
}

BaseType_t kraken_receive_message(QueueHandle_t queue, void* buffer, TickType_t timeout) {
    return xQueueReceive(queue, buffer, timeout);
}

TimerHandle_t kraken_create_timer(const char* name, TickType_t period, BaseType_t autoReload, void* id, TimerCallbackFunction_t callback) {
    return xTimerCreate(name, period, autoReload, id, callback);
}
