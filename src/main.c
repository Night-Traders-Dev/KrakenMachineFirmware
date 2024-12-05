#include "kraken_scheduler.h"
#include "bsp/board.h"
#include "tusb.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include <stdlib.h>

#define PICO_DEFAULT_LED_PIN 25
#define EVENT_LED_ON_BIT (1 << 0)
#define EVENT_LED_OFF_BIT (1 << 1)

/* Global Event Group */
static EventGroupHandle_t led_event_group;

/* LED Blink Task */
void led_blink_task(void *parameters) {
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    while (1) {
        /* Wait for the LED ON bit to be set */
        EventBits_t events = kraken_wait_for_event_bits(led_event_group, EVENT_LED_ON_BIT, pdTRUE, pdFALSE, portMAX_DELAY);

        if (events & EVENT_LED_ON_BIT) {
            gpio_put(PICO_DEFAULT_LED_PIN, 1);
            kraken_log("LED ON");
            vTaskDelay(pdMS_TO_TICKS(500));
            /* Set the LED OFF bit */
            kraken_set_event_bits(led_event_group, EVENT_LED_OFF_BIT);
            vTaskDelay(pdMS_TO_TICKS(5000));
        }
    }
}

/* Controller Task */
void controller_task(void *parameters) {
    while (1) {
        /* Set the LED ON bit */
        kraken_set_event_bits(led_event_group, EVENT_LED_ON_BIT);

        /* Wait for the LED OFF bit */
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
        /* Fetch and print task runtime stats */
        kraken_print_task_runtime_stats(stats_buffer, sizeof(stats_buffer));
        printf("Runtime Stats:\n%s\n", stats_buffer);

        vTaskDelay(pdMS_TO_TICKS(5000)); // Print every 5 seconds
    }
}



int main(void) {
    /* Initialize stdio */
    stdio_init_all();
    board_init();
    tusb_init();

    /* Initialize the Kraken log system */
    kraken_log("Initializing Kraken Machine Firmware...");

    /* Create the Event Group */
    led_event_group = kraken_create_event_group();
    if (led_event_group == NULL) {
        kraken_assert(0, "Failed to create event group");
    }

    /* Create Tasks */
    kraken_create_task(led_blink_task, "LED Blink", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
    kraken_create_task(controller_task, "Controller", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
    kraken_create_task(runtime_stats_task, "Runtime Stats", configMINIMAL_STACK_SIZE * 2, NULL, tskIDLE_PRIORITY + 1, NULL);

    /* Start the Scheduler */
    kraken_start_scheduler();

    /* Should never reach here */
    kraken_assert(0, "Scheduler stopped unexpectedly");
    while (1);
}
