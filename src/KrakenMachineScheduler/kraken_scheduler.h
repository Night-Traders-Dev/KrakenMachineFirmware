#ifndef KRAKEN_SCHEDULER_H
#define KRAKEN_SCHEDULER_H

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

/* Task Management */
BaseType_t kraken_create_task(TaskFunction_t pxTaskCode, const char *pcName, uint16_t usStackDepth, void *pvParameters, UBaseType_t uxPriority, TaskHandle_t *pxCreatedTask);
void kraken_delete_task(TaskHandle_t xTask);
void kraken_suspend_task(TaskHandle_t xTask);
void kraken_resume_task(TaskHandle_t xTask);
TaskHandle_t kraken_get_current_task(void);

/* Task Priority Management */
void kraken_set_task_priority(TaskHandle_t xTask, UBaseType_t uxNewPriority);
UBaseType_t kraken_get_task_priority(TaskHandle_t xTask);

/* Task Notifications */
void kraken_notify_task(TaskHandle_t xTask, uint32_t value);
void kraken_wait_for_notification(uint32_t *notificationValue, TickType_t xTicksToWait);

/* Semaphore Management */
SemaphoreHandle_t kraken_create_binary_semaphore(void);
SemaphoreHandle_t kraken_create_counting_semaphore(uint32_t maxCount, uint32_t initialCount);
void kraken_give_semaphore(SemaphoreHandle_t xSemaphore);
void kraken_take_semaphore(SemaphoreHandle_t xSemaphore, TickType_t xTicksToWait);

/* Mutex Management */
SemaphoreHandle_t kraken_create_mutex(void);
void kraken_take_mutex(SemaphoreHandle_t xMutex, TickType_t xTicksToWait);
void kraken_give_mutex(SemaphoreHandle_t xMutex);

/* Timer Management */
TimerHandle_t kraken_create_timer(const char *name, TickType_t period, bool autoReload, void *timerID, TimerCallbackFunction_t callback);
void kraken_start_timer(TimerHandle_t xTimer);
void kraken_stop_timer(TimerHandle_t xTimer);

/* Diagnostics */
UBaseType_t kraken_get_number_of_tasks(void);
void kraken_get_task_stats(char *buffer, size_t bufferSize);
size_t kraken_get_free_heap_size(void);

/* FreeRTOS Hook Functions */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize);
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize);
void vApplicationMallocFailedHook(void);
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName);

/* Debug Logging */
void kraken_log(const char *message);

#endif // KRAKEN_SCHEDULER_H
