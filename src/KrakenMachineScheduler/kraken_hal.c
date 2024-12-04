#include "kraken_hal.h"

// GPIO management
void kraken_gpio_init(uint pin, bool output) {
    gpio_init(pin);
    if (output) {
        gpio_set_dir(pin, GPIO_OUT);
    } else {
        gpio_set_dir(pin, GPIO_IN);
    }
}

void kraken_gpio_write(uint pin, bool value) {
    gpio_put(pin, value);
}

bool kraken_gpio_read(uint pin) {
    return gpio_get(pin);
}

// UART
void kraken_uart_init(uint baudrate) {
    stdio_init_all();
    uart_init(uart0, baudrate);
    gpio_set_function(PICO_DEFAULT_UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(PICO_DEFAULT_UART_RX_PIN, GPIO_FUNC_UART);
}

void kraken_uart_write(const char* data) {
    uart_puts(uart0, data);
}

char kraken_uart_read() {
    return uart_getc(uart0);
}

// SPI
void kraken_spi_init(uint baudrate) {
    spi_init(spi0, baudrate);
    gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI);
}

void kraken_spi_write(const uint8_t* data, size_t length) {
    spi_write_blocking(spi0, data, length);
}

void kraken_spi_read(uint8_t* buffer, size_t length) {
    spi_read_blocking(spi0, 0x00, buffer, length);
}
