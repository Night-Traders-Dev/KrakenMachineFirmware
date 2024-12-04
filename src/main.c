#include "kraken_scheduler.h"
#include "stackctrl.h"
#include "runtime.h"
#include "repl.h"
#include "compile.h"
#include "pyexec.h"

#include <stdio.h>
#include <stdlib.h>

#define PICO_DEFAULT_LED_PIN 25

/* Task to run MicroPython */
void mp_task(void *pvParameters) {
    mp_stack_ctrl_init();
    mp_stack_set_limit(10240);
    mp_stack_ctrl_init();

    gc_init((void *)0x20000000, (void *)0x20002000);
    mp_init();
    mp_obj_list_init(mp_sys_path, 0);
    mp_obj_list_init(mp_sys_argv, 0);

    printf("MicroPython loaded...\n");

    // Run the REPL
    pyexec_friendly_repl();

    // Cleanup
    mp_deinit();
    vTaskDelete(NULL);
}

void led_blink_task(void *parameters) {
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    while (1) {
        gpio_put(PICO_DEFAULT_LED_PIN, 1);
        vTaskDelay(pdMS_TO_TICKS(500));
        gpio_put(PICO_DEFAULT_LED_PIN, 0);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

int main(void) {
    stdio_init_all();

    // Create LED blink task
    kraken_create_task(led_blink_task, "LED Blink", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);

    // Create MicroPython REPL task
    kraken_create_task(mp_task, "MicroPython REPL", 8192, NULL, tskIDLE_PRIORITY + 2, NULL);

    // Start the FreeRTOS scheduler
    vTaskStartScheduler();

    while (1); // Should never reach here
}
