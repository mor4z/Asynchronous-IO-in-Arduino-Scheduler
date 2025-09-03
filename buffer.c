#include "buffer.h"
#include "scheduler.h"

#include <avr/interrupt.h>
#include <avr/io.h>

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

void bufferInfo(RingBuffer* buffer) {
    // buffer = NULL;
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

void bufferWrite(RingBuffer* buffer, char c) {
    if ((buffer -> head + 1) % BUFFER_SIZE == (buffer -> tail)) {
        printf("[bufferWrite] Buffer pieno, impossibile scrivere\n");
        return;
    }

    buffer -> data[buffer -> head] = c;
    buffer -> head = (buffer -> head + 1) % (BUFFER_SIZE);
    buffer -> size++;
    // printf("[bufferWrite] Scrittura su buffer completata con successo\n");
}

char bufferRead(RingBuffer* buffer) {
    if (buffer -> head == buffer -> tail) {
        printf("[bufferRead] Buffer vuoto, impossibile leggere\n");
        return -1;
    }

    char c = buffer -> data[buffer -> tail];
    buffer -> data[buffer -> tail] = 0;
    buffer -> tail = (buffer -> tail + 1) % BUFFER_SIZE;
    buffer -> size--;
    // printf("[bufferRead] Lettura da buffer completata con successo\n");
    return c;
} 

// funzione per attivare interrupt in ricezione
void enableRxInterrupt(void){   
    UCSR0B |= (1<<RXCIE0);
}

/* ***********Funzioni richieste dal professore ********** 
char getChar(), that reads, if available a character from the input buffer. If the buffer is empty the "thread" asking for the character is put in a waiting queue. When a character becomes available the thread is brought back in running, and the character is returned (and consumed from the buffer)

void putChar(char), that writes (if there is enough space) a character on the output buffer. If the buffer is full, the thread is put in waiting, and resumed whenever there is enough room in the buffer.
*/

char getChar(void) {
    // Se il buffer di lettura è vuoto, metto il thread corrente nella coda di attesa di lettura
    if (inputBuffer.size == 0) {
        TCBList_enqueue(&reading_queue, current_tcb);
        current_tcb -> status = Waiting;
        schedule();
    }

    // Se c'è qualcosa nel buffer di lettura, leggo e ritorno il carattere nel buffer
    char c = bufferRead(&inputBuffer);
    return c;
}

void putChar(char c) {
    // Se il buffer di scrittura è pieno, metto il thread corrente nella coda di attesa di scrittura
    if (outputBuffer.size == BUFFER_SIZE) {
        TCBList_enqueue(&writing_queue, current_tcb);
        current_tcb -> status = Waiting;
        schedule();
    }

    // Se non è pieno il buffer di scrittura, scrivo il carattere
    bufferWrite(&outputBuffer, c);
    return;
}

/* TODO: mancano le due funzioni che vanno a mettere in coda di ready i task se i buffer di input o di output non sono vuoti */
