#include "functions.h"
// #include "scheduler.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/atomic.h>
#include <util/delay.h>


// Inizializzo un buffer vuoto
void bufferInit(RingBuffer* buffer) {
    if (buffer == NULL) {
        // printf("[bufferInit -> Errore] buffer == NULL\n");
        return;
    }

    buffer -> head = 0;
    buffer -> tail = 0;
    buffer -> size = 0;

    // Metto a 0 tutti i caratteri del buffer
    for (int i = 0; i < BUFFER_SIZE; i++) {
        buffer -> data[i] = 0;
    }

    // sprintf(tx, "Init OK\n");
    // printString(tx);
    // _delay_ms(10); 
} 


// Scrittura di un carattere su un buffer
void bufferWrite(RingBuffer* buffer, char c) {

    // Aggiornamento dei campi
    buffer -> data[buffer -> head] = c;
    buffer -> head = (buffer -> head + 1) % (BUFFER_SIZE);
    buffer -> size++;
    // printf("[bufferWrite] Scrittura su buffer completata con successo\n");
}

// Lettura di un carattere da un buffer
char bufferRead(RingBuffer* buffer) {

    // Aggiornamento dei campi
    char c = buffer -> data[buffer -> tail];
    buffer -> data[buffer -> tail] = 0;
    buffer -> tail = (buffer -> tail + 1) % BUFFER_SIZE;
    buffer -> size--;
    // printf("[bufferRead] Lettura da buffer completata con successo\n");
    return c;
} 

// Funzione per attivare interrupt in ricezione della seriale
void enableRxInterrupt(void) {   
    UCSR0B |= (1<<RXCIE0);
}


/* ***********Funzioni richieste dal professore ********** 
char getChar(), that reads, if available a character from the input buffer. If the buffer is empty the "thread" asking for the character is put in a waiting queue. When a character becomes available the thread is brought back in running, and the character is returned (and consumed from the buffer)

void putChar(char), that writes (if there is enough space) a character on the output buffer. If the buffer is full, the thread is put in waiting, and resumed whenever there is enough room in the buffer.
*/

char getChar(void) {
    if (inputBuffer.size == 0)
        return 0;
    char c;
    ATOMIC_BLOCK(ATOMIC_FORCEON) {
        c = bufferRead(&inputBuffer);
    }
    return c;
}

void putChar(char c) {
    while(outputBuffer.size >= BUFFER_SIZE);
    ATOMIC_BLOCK(ATOMIC_FORCEON) {
        bufferWrite(&outputBuffer, c);
    }
    UCSR0B |= _BV(UDRIE0); // Abilito interrupt di trasmissione
}


