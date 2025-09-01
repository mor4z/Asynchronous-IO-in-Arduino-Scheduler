#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define BUFFER_SIZE 64

typedef struct {
    char data[BUFFER_SIZE];
    uint8_t head;               // Indice per la scrittura (ultimo carattere inserito)
    uint8_t tail;               // Indice per la lettura (primo carattere inserito)
    uint8_t size;               // Dimensione del buffer
    uint8_t free_space;         // Potrebbe essere utile, vedremo
} RingBuffer;


void bufferInit(RingBuffer* buffer);                // Inizializzazione del buffer
void bufferInfo(RingBuffer* buffer);                // Stampa i campi del buffer (per debugging)
void bufferWrite(RingBuffer* buffer, char c);    // Scrittura di un carattere sul buffer
char bufferRead(RingBuffer* buffer);                // Lettura di un carattere dal buffer
