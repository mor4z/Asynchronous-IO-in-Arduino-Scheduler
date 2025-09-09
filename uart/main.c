#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "uart.h"
#include "ringbuffer.h"

RingBuffer inputBuffer;
RingBuffer outputBuffer;

#define MAX_MSG_LEN 128

int main(void) {
    usart_init(MYUBRR);
    sei(); 

    // Inizializzo i buffer
    bufferInit(&inputBuffer);
    bufferInit(&outputBuffer);

    // LED su pin 13 (PB7) per debugging
    DDRB |= (1 << PB7);
    PORTB &= ~(1 << PB7);

    uart_pstr("UART pronta\r\n");

    char msg[MAX_MSG_LEN];
    uint8_t idx = 0;

    while (1) {
        if (uart_available()) {
            char c = getChar();

            // Se ricevo newline mando il messaggio completo
            if (c == '\n' || c == '\r') {
                msg[idx] = '\0'; // termina stringa
                uart_pstr("Ricevuto: ");
                uart_pstr(msg);
                uart_pstr("\r\n");

                // Reset indice per il prossimo messaggio
                idx = 0;
            } else {
                // Scrivo nel buffer i caratteri
                if (idx < (MAX_MSG_LEN - 1)) {
                    msg[idx++] = c; 
                }
            }
        }
        _delay_ms(5);
    }
}
