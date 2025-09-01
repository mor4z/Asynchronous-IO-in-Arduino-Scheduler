#include "buffer.h"

void bufferInit(RingBuffer* buffer) {
    if (buffer == NULL) {
        printf("[bufferInit -> Errore] buffer == NULL\n");
        return;
    }

    buffer -> head = 0;
    buffer -> tail = 0;
    buffer -> size = BUFFER_SIZE;

    // Metto a 0 tutti i caratteri del buffer
    for (int i = 0; i < buffer -> size; i++) {
        buffer -> data[i] = 0;
    } 
    
    printf("[bufferInit] Buffer creato con successo\n");
} 

void bufferInfo(RingBuffer* buffer) {
    // buffer = NULL;
    if (buffer == NULL) {
        printf("[bufferInfo -> Errore!]Buffer NULL\n");
        return;
    }

    // Stampa del contenuto di data
    printf("[buffer -> data]");
    for (int k = 0; k < buffer -> size; k++)
        printf("%c", buffer -> data[k]);
    printf("\n");

    // Stampa degli altri campi
    printf("[buffer -> head] %u \n[buffer -> tail] %u \n[buffer -> size] %u \n", buffer -> head, buffer -> tail, buffer -> size);
}

void bufferWrite(RingBuffer* buffer, char c) {
    if ((buffer -> head + 1) % (buffer -> size) == (buffer -> tail)) {
        printf("[bufferWrite] Buffer pieno, impossibile scrivere\n");
        return;
    }

    buffer -> data[buffer -> head] = c;
    buffer -> head = (buffer -> head + 1) % (buffer -> size);
    // printf("[bufferWrite] Scrittura su buffer completata con successo\n");
}

char bufferRead(RingBuffer* buffer) {
    if (buffer -> head == buffer -> tail) {
        printf("[bufferRead] Buffer vuoto, impossibile leggere\n");
        return -1;
    }

    char c = buffer -> data[buffer -> tail];
    buffer -> data[buffer -> tail] = 0;
    buffer -> tail = (buffer -> tail + 1) % (buffer -> size);
    // printf("[bufferRead] Lettura da buffer completata con successo\n");
    return c;
} 

