#include "uart.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "ringbuffer.h"   // il file dove hai definito RingBuffer e le funzioni

extern RingBuffer inputBuffer;
extern RingBuffer outputBuffer;

void usart_init( uint16_t ubrr) {
    // Set baud rate
    UBRR0H = (uint8_t)(ubrr>>8);
    UBRR0L = (uint8_t)ubrr;

    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */ 
    UCSR0B = _BV(RXEN0) | _BV(TXEN0) | _BV(RXCIE0);   /* Enable RX and TX */  
}

uint8_t uart_available(void) {
    if (inputBuffer.size > 0)
        return 1;
    else
        return 0;
}

void uart_pstr(const char *s) {
    while (*s) {
        putChar(*s++);
    }
}

// ISR di ricezione
ISR(USART0_RX_vect) {
    PORTB |= _BV(PB7); // debugging
    char c = UDR0;
    bufferWrite(&inputBuffer, c);
}

// ISR di trasmissione (Data Register Empty)
ISR(USART0_UDRE_vect) {
    if (outputBuffer.size == 0) {
        // niente da trasmettere disabilito interrupt di trasmissione
        UCSR0B &= ~_BV(UDRIE0);
    } else {
        char c = bufferRead(&outputBuffer);
        UDR0 = c;
        PORTB &= ~(1 << PB7); // debugging
    } 
}

