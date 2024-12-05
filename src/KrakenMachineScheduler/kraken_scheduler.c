#include "kraken_scheduler.h"
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include <stdio.h>
#include <stdlib.h>

/* Static allocation for the Idle task */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

/* Static allocation for the Timer task */
static StaticTask_t xTimerTaskTCBBuffer;
static StackType_t xTimerStack[configTIMER_TASK_STACK_DEPTH];

/* Task Management */
BaseType_t kraken_create_task(TaskFunction_t pxTaskCode, const char *pcName, uint16_t usStackDepth, void *pvParameters, UBaseType_t uxPriority, TaskHandle_t *pxCreatedTask) {
    return xTaskCreate(pxTaskCode, pcName, usStackDepth, pvParameters, uxPriority, pxCreatedTask);
}

void kraken_delete_task(TaskHandle_t xTask) {
    vTaskDelete(xTask);
}

void kraken_suspend_task(TaskHandle_t xTask) {
    vTaskSuspend(xTask);
}

void kraken_resume_task(TaskHandle_t xTask) {
    vTaskResume(xTask);
}

TaskHandle_t kraken_get_current_task(void) {
    return xTaskGetCurrentTaskHandle();
}

/* Task Priority Management */
void kraken_set_task_priority(TaskHandle_t xTask, UBaseType_t uxNewPriority) {
    vTaskPrioritySet(xTask, uxNewPriority);
}

UBaseType_t kraken_get_task_priority(TaskHandle_t xTask) {
    return uxTaskPriorityGet(xTask);
}

/* Task Notifications */
void kraken_notify_task(TaskHandle_t xTask, uint32_t value) {
    xTaskNotify(xTask, value, eSetValueWithOverwrite);
}

void kraken_wait_for_notification(uint32_t *notificationValue, TickType_t xTicksToWait) {
    xTaskNotifyWait(0, 0, notificationValue, xTicksToWait);
}

/* Semaphore Management */
SemaphoreHandle_t kraken_create_binary_semaphore(void) {
    return xSemaphoreCreateBinary();
}

SemaphoreHandle_t kraken_create_counting_semaphore(uint32_t maxCount, uint32_t initialCount) {
    return xSemaphoreCreateCounting(maxCount, initialCount);
}

void kraken_give_semaphore(SemaphoreHandle_t xSemaphore) {
    xSemaphoreGive(xSemaphore);
}

void kraken_take_semaphore(SemaphoreHandle_t xSemaphore, TickType_t xTicksToWait) {
    xSemaphoreTake(xSemaphore, xTicksToWait);
}

/* Mutex Management */
SemaphoreHandle_t kraken_create_mutex(void) {
    return xSemaphoreCreateMutex();
}

void kraken_take_mutex(SemaphoreHandle_t xMutex, TickType_t xTicksToWait) {
    xSemaphoreTake(xMutex, xTicksToWait);
}

void kraken_give_mutex(SemaphoreHandle_t xMutex) {
    xSemaphoreGive(xMutex);
}

/* Timer Management */
TimerHandle_t kraken_create_timer(const char *name, TickType_t period, bool autoReload, void *timerID, TimerCallbackFunction_t callback) {
    return xTimerCreate(name, period, autoReload, timerID, callback);
}

void kraken_start_timer(TimerHandle_t xTimer) {
    xTimerStart(xTimer, 0);
}

void kraken_stop_timer(TimerHandle_t xTimer) {
    xTimerStop(xTimer, 0);
}

/* Diagnostics */
UBaseType_t kraken_get_number_of_tasks(void) {
    return uxTaskGetNumberOfTasks();
}

void kraken_get_task_stats(char *buffer, size_t bufferSize) {
    vTaskList(buffer); // Ensure the buffer is large enough to hold task stats
}

size_t kraken_get_free_heap_size(void) {
    return xPortGetFreeHeapSize();
}

/* FreeRTOS Hook Functions */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize) {
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
    *ppxIdleTaskStackBuffer = xIdleStack;
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize) {
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCBBuffer;
    *ppxTimerTaskStackBuffer = xTimerStack;
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}

void vApplicationMallocFailedHook(void) {
    panic("malloc failed");
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    (void)pcTaskName;
    (void)xTask;
    for (;;); // Infinite loop
}

/* Debug Logging */
void kraken_log(const char *message) {
    printf("[KRAKEN_SCHEDULER] %s\n", message);
}
