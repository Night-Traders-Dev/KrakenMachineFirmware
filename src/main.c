#include "kraken_scheduler.h"
#include "pico/stdlib.h"
//#include "tusb.h"
#include <stdio.h>
#include <stdlib.h>

#define PICO_DEFAULT_LED_PIN 25
#define EVENT_LED_ON_BIT (1 << 0)
#define EVENT_LED_OFF_BIT (1 << 1)

/* Global Event Group */
static EventGroupHandle_t led_event_group;
static uint32_t blink_interval_ms = 250; // Default to BLINK_NOT_MOUNTED

/* USB Device Task */
void usb_device_task(void *parameters) {
    while (1) {
        tud_task(); // TinyUSB device task
        vTaskDelay(pdMS_TO_TICKS(10)); // Avoid tight loop
    }
}

/* LED Blink Task */
void led_blink_task(void *parameters) {
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    while (1) {
        EventBits_t events = kraken_wait_for_event_bits(led_event_group, EVENT_LED_ON_BIT, pdTRUE, pdFALSE, portMAX_DELAY);

        if (events & EVENT_LED_ON_BIT) {
            gpio_put(PICO_DEFAULT_LED_PIN, 1);
            kraken_log("LED ON");
            vTaskDelay(pdMS_TO_TICKS(blink_interval_ms));
            kraken_set_event_bits(led_event_group, EVENT_LED_OFF_BIT);
        }
    }
}

/* Controller Task */
void controller_task(void *parameters) {
    while (1) {
        kraken_set_event_bits(led_event_group, EVENT_LED_ON_BIT);

        EventBits_t events = kraken_wait_for_event_bits(led_event_group, EVENT_LED_OFF_BIT, pdTRUE, pdFALSE, portMAX_DELAY);

        if (events & EVENT_LED_OFF_BIT) {
            gpio_put(PICO_DEFAULT_LED_PIN, 0);
            kraken_log("LED OFF");
            vTaskDelay(pdMS_TO_TICKS(500));
        }
    }
}

/* Runtime Stats Task */
void runtime_stats_task(void *parameters) {
    char stats_buffer[512];

    while (1) {
        kraken_print_task_runtime_stats(stats_buffer, sizeof(stats_buffer));
        printf("Runtime Stats:\n%s\n", stats_buffer);

        vTaskDelay(pdMS_TO_TICKS(5000)); // Print every 5 seconds
    }
}

/* USB Device Callbacks */
void tud_mount_cb(void) {
    blink_interval_ms = 1000; // BLINK_MOUNTED
    kraken_log("Device Mounted");
}

void tud_umount_cb(void) {
    blink_interval_ms = 250; // BLINK_NOT_MOUNTED
    kraken_log("Device Unmounted");
}

void tud_suspend_cb(bool remote_wakeup_en) {
    (void) remote_wakeup_en;
    blink_interval_ms = 2500; // BLINK_SUSPENDED
    kraken_log("Device Suspended");
}

void tud_resume_cb(void) {
    blink_interval_ms = tud_mounted() ? 1000 : 250; // BLINK_MOUNTED or BLINK_NOT_MOUNTED
    kraken_log("Device Resumed");
}

/* Main Function */
int main(void) {
    kraken_log("Initializing Kraken Machine Firmware...");

    /* Create the Event Group */
    led_event_group = kraken_create_event_group();
    if (led_event_group == NULL) {
        kraken_assert(0, "Failed to create event group");
    }

    /* Initialize TinyUSB */
    tud_init(BOARD_TUD_RHPORT);

    /* Create Tasks */
    kraken_create_task(usb_device_task, "USB Device", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
    kraken_create_task(led_blink_task, "LED Blink", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
    kraken_create_task(controller_task, "Controller", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
    kraken_create_task(runtime_stats_task, "Runtime Stats", configMINIMAL_STACK_SIZE * 2, NULL, tskIDLE_PRIORITY + 1, NULL);

    /* Start the Scheduler */
    kraken_start_scheduler();

    /* Should never reach here */
    kraken_assert(0, "Scheduler stopped unexpectedly");
    while (1);
}
