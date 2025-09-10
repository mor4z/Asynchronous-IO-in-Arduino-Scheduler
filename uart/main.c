#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "uart.h"
#include "ringbuffer.h"

RingBuffer inputBuffer;
RingBuffer outputBuffer;

volatile uint8_t a;
volatile uint8_t PrintFlag;
volatile char buf[BUFFER_SIZE];

int main(void) {
    printf_init();
    sei();

    // Inizializzo i buffer
    bufferInit(&inputBuffer);
    bufferInit(&outputBuffer);

    // LED su pin 13 (PB7) per debugging
    DDRB |= (1 << PB7);
    PORTB &= ~(1 << PB7);

    usart_pstr("UART pronta\n");

    a = 1;
    PrintFlag = 0;

    while(1) {

        while (inputBuffer.size > 0) {
            // printf("size = %d\n", inputBuffer.size);
            char c = getChar();

            putChar(c);
            _delay_ms(10);
        }
        
        _delay_ms(1000);

        // bufferInfo(&outputBuffer);
        
        printf("a = %d\n", a);

        if (a >= BUFFER_SIZE)
            break;

    } 

    if (PrintFlag == 0) {
        printf("Spazio esaurito\n");
    }
    
    // stampa dell'output buffer
    while (outputBuffer.size > 0) {
        char c = bufferRead(&outputBuffer);
        printf("%c", c);
    }
    printf("\nFine stampa");
    
    printf("\n\nProgramma terminato\n");

    bufferInfo(&outputBuffer);
}
