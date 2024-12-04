#include "pico/stdlib.h"
#include "kraken_scheduler.h"
#include "kraken_hal.h"


#define PICO_DEFAULT_LED_PIN 25

static void led_blink(void *parameters) {
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    while (1) {
        vTaskDelay(500);
        gpio_put(PICO_DEFAULT_LED_PIN, 1);
        vTaskDelay(500);
        gpio_put(PICO_DEFAULT_LED_PIN, 0);
    }
}

int main(void) {
    stdio_init_all();
    kraken_uart_init(115200);

    kraken_uart_write("Starting Kraken Machine...\n");
    printf("Starting Kraken Machine...\n");

    /* Create LED blink task using KrakenScheduler */
    kraken_create_task(led_blink, "LED Task", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);

    /* Start the scheduler */
    vTaskStartScheduler();

    /* Should never reach here */
    panic_unsupported();
}
