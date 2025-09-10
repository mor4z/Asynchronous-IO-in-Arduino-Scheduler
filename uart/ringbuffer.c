#include "ringbuffer.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/atomic.h>

// Inizializzo un buffer vuoto
void bufferInit(RingBuffer* buffer) {
    if (buffer == NULL) {
        printf("[bufferInit -> Errore] buffer == NULL\n");
        return;
    }

    buffer -> head = 0;
    buffer -> tail = 0;
    buffer -> size = 0;

    // Metto a 0 tutti i caratteri del buffer
    for (int i = 0; i < BUFFER_SIZE; i++) {
        buffer -> data[i] = 0;
    } 
    
    printf("[bufferInit] Buffer creato con successo\n");
} 

// Stampo i campi di un buffer
void bufferInfo(RingBuffer* buffer) {
    if (buffer == NULL) {
        printf("[bufferInfo -> Errore!]Buffer NULL\n");
        return;
    }

    // Stampa del contenuto di data
    printf("[buffer -> data]");
    for (int k = 0; k < BUFFER_SIZE; k++)
        printf("%c", buffer -> data[k]);
    printf("\n");

    // Stampa degli altri campi
    printf("[buffer -> head] %u \n[buffer -> tail] %u \n[buffer -> size] %u \n", buffer -> head, buffer -> tail, buffer -> size);
}

// Scrittura di un carattere su un buffer
void bufferWrite(RingBuffer* buffer, char c) {
    if ((buffer -> head + 1) % BUFFER_SIZE == (buffer -> tail)) {
        printf("[bufferWrite] Buffer pieno, impossibile scrivere\n");
        return;
    }

    // Aggiornamento dei campi
    buffer -> data[buffer -> head] = c;
    buffer -> head = (buffer -> head + 1) % (BUFFER_SIZE);
    buffer -> size++;
    // printf("[bufferWrite] Scrittura su buffer completata con successo\n");
}

// Lettura di un carattere da un buffer
char bufferRead(RingBuffer* buffer) {
    if (buffer -> head == buffer -> tail) {
        printf("[bufferRead] Buffer vuoto, impossibile leggere\n");
        return -1;
    }

    // Aggiornamento dei campi
    char c = buffer -> data[buffer -> tail];
    buffer -> data[buffer -> tail] = 0;
    buffer -> tail = (buffer -> tail + 1) % BUFFER_SIZE;
    buffer -> size--;
    // printf("[bufferRead] Lettura da buffer completata con successo\n");
    return c;
} 


// Funzioni richieste dal professore
char getChar() {
    while(inputBuffer.size == 0);   // Attendo un carattere
    char c;
    ATOMIC_BLOCK(ATOMIC_FORCEON) {
        c = bufferRead(&inputBuffer);
    }
    return c;
}

void putChar(char c) {
    ATOMIC_BLOCK(ATOMIC_FORCEON) {
        bufferWrite(&outputBuffer, c);
    }
    UCSR0B |= _BV(UDRIE0); // Attiva interrupt di trasmissione
}