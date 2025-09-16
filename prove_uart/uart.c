#include "uart.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/atomic.h>
#include "functions.h"
#include <util/setbaud.h>


extern RingBuffer inputBuffer;
extern RingBuffer outputBuffer;

void usart_init(int ubrr) {
    // Set baud rate
    UBRR0H = (uint8_t)(ubrr>>8);
    UBRR0L = (uint8_t)ubrr;

    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */ 
    UCSR0B = _BV(RXEN0) | _BV(TXEN0) | _BV(RXCIE0);   /* Enable RX and TX */  
}

void usart_putchar(char data) {
    // Wait for empty transmit buffer
     while ( !(UCSR0A & (1<<UDRE0)) );
    // Start transmission
    UDR0 = data; 
}

char usart_getchar(void) {
    // Wait for incoming data
    while ( !(UCSR0A & (1<<RXC0)) );
    // Return the data
    return UDR0;
}

void usart_pstr(char *s) {
    // loop through entire string
    while (*s) { 
        usart_putchar(*s);
        s++;
    }
}
 

// ISR per la ricezione da seriale
ISR(USART0_RX_vect) {
  PORTB |= _BV(PB7); // debugging

  // Scrittura del carattere ricevuto nel buffer
  char c = UDR0;
  if (inputBuffer.size < BUFFER_SIZE)
    bufferWrite(&inputBuffer, c);

}


// ISR per la trasmissione da seriale
ISR(USART0_UDRE_vect) {
  if (!outputBuffer.size) {
    UCSR0B &= ~_BV(UDRIE0);
  } else {
    UDR0 = bufferRead(&outputBuffer);
  }
  PORTB &= ~(1 << PB7); // debugging
}