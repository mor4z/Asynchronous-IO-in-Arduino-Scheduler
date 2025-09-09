#ifndef UART_RINGBUFFER_H
#define UART_RINGBUFFER_H

#include <stdint.h>
#include <stdbool.h>

#define F_CPU 16000000UL
#define BAUD 57600
#define MYUBRR (F_CPU/16/BAUD - 1)

void usart_init(uint16_t ubrr);
void uart_putchar(char c);
char uart_getchar(void);
uint8_t uart_available(void);
void uart_pstr(const char *s);

#endif