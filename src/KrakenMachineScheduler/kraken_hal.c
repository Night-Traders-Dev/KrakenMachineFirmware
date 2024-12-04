#include "kraken_hal.h"
#include "pico/stdlib.h"

/* GPIO Functions */

void kraken_gpio_init(uint32_t pin, bool is_output) {
    gpio_init(pin);
    gpio_set_dir(pin, is_output);
}

void kraken_gpio_write(uint32_t pin, bool value) {
    gpio_put(pin, value);
}

bool kraken_gpio_read(uint32_t pin) {
    return gpio_get(pin);
}

/* Timer Functions */

void kraken_delay_ms(uint32_t ms) {
    sleep_ms(ms);
}

void kraken_delay_us(uint32_t us) {
    sleep_us(us);
}

/* UART Functions */

void kraken_uart_init(uint32_t baud_rate) {
    stdio_init_all();
    uart_init(uart0, baud_rate);

    // Set GPIO pins for UART (assumes default UART0 pins for RP2040)
    gpio_set_function(0, GPIO_FUNC_UART);
    gpio_set_function(1, GPIO_FUNC_UART);
}

void kraken_uart_write(const char *data) {
    while (*data) {
        uart_putc(uart0, *data++);
    }
}

void kraken_uart_write_byte(uint8_t byte) {
    uart_putc(uart0, byte);
}
