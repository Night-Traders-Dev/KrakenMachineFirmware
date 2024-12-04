#ifndef KRAKEN_HAL_H
#define KRAKEN_HAL_H

#include <stdint.h> // For uint32_t, uint8_t
#include <stdbool.h> // For bool

/* GPIO Functions */
void kraken_gpio_init(uint32_t pin, bool is_output);
void kraken_gpio_write(uint32_t pin, bool value);
bool kraken_gpio_read(uint32_t pin);

/* Timer Functions */
void kraken_delay_ms(uint32_t ms);
void kraken_delay_us(uint32_t us);

/* UART Functions */
void kraken_uart_init(uint32_t baud_rate);
void kraken_uart_write(const char *data);
void kraken_uart_write_byte(uint8_t byte);

#endif // KRAKEN_HAL_H
