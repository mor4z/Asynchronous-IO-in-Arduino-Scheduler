#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define BUFFER_SIZE 256

typedef struct {
    char data[BUFFER_SIZE];
    uint8_t head;               // Indice per la scrittura (ultimo carattere inserito)
    uint8_t tail;               // Indice per la lettura (primo carattere inserito)
    uint8_t size;               // Spazio occupato nel buffer
} RingBuffer;


void bufferInit(RingBuffer* buffer);                // Inizializzazione del buffer
void bufferInfo(RingBuffer* buffer);                // Stampa i campi del buffer (per debugging)
void bufferWrite(RingBuffer* buffer, char c);       // Scrittura di un carattere sul buffer
char bufferRead(RingBuffer* buffer);                // Lettura di un carattere dal buffer

RingBuffer inputBuffer;
RingBuffer outputBuffer;

// Funzioni richieste dal professore
char getChar(void);
void putChar(char c);

void printString(char* s);

// Funzioni che mettono in esecuzione i task dalle code di attesa
uint8_t checkInput(void);
uint8_t checkOutput(void);

void enableRxInterrupt(void);
