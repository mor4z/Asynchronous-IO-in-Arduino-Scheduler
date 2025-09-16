#include "uart.h"
#include "functions.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>

char rx[BUFFER_SIZE];
char tx[BUFFER_SIZE];

void printString(char* s){
  int l=strlen(s);
  for(int i=0; i<l; ++i, ++s) {
    putChar(*s);
  }  
}

int main(void) {
    usart_init(MYUBRR);
    sei();

    enableRxInterrupt();
    
    bufferInit(&inputBuffer);
    bufferInit(&outputBuffer);

    sprintf(tx, "Starting\n");
    printString(tx);

    while(1) {
        rx[0] = 0;
        int size = 0;
        while(1) {
            sprintf(tx, "RX: %s \n", rx);
            if (strlen(rx) > 1)
                printString(tx);
            _delay_ms(10);

            char c = getChar();
            rx[size]=c;
            ++size;
            rx[size]=0;
            
            if (c == 0xFF ||c == '\n' || c=='\r' || c==0) {
                break;
            }
        }
    }
    
}

