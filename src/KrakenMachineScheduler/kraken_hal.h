#ifndef KRAKEN_HAL_H
#define KRAKEN_HAL_H

#include "pico/stdlib.h"

// GPIO management
void kraken_gpio_init(uint pin, bool output);
void kraken_gpio_write(uint pin, bool value);
bool kraken_gpio_read(uint pin);

// UART
void kraken_uart_init(uint baudrate);
void kraken_uart_write(const char* data);
char kraken_uart_read();

// SPI
void kraken_spi_init(uint baudrate);
void kraken_spi_write(const uint8_t* data, size_t length);
void kraken_spi_read(uint8_t* buffer, size_t length);

#endif // KRAKEN_HAL_H
