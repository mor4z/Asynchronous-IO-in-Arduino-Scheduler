#include "functions.h"
#include "scheduler.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/atomic.h>
#include <string.h>

// Inizializzo un buffer vuoto
void bufferInit(RingBuffer* buffer) {
    if (buffer == NULL) {
        return;
    }

    buffer -> head = 0;
    buffer -> tail = 0;
    buffer -> size = 0;

    // Metto a 0 tutti i caratteri del buffer
    for (int i = 0; i < BUFFER_SIZE; i++) {
        buffer -> data[i] = 0;
    } 
    
} 

// Scrittura di un carattere su un buffer
void bufferWrite(RingBuffer* buffer, char c) {
    if ((buffer -> head + 1) % BUFFER_SIZE == (buffer -> tail)) {
        return;
    }

    // Aggiornamento dei campi
    buffer -> data[buffer -> head] = c;
    buffer -> head = (buffer -> head + 1) % (BUFFER_SIZE);
    buffer -> size++;
}

// Lettura di un carattere da un buffer
char bufferRead(RingBuffer* buffer) {
    if (buffer -> head == buffer -> tail) {
        return -1;
    }

    // Aggiornamento dei campi
    char c = buffer -> data[buffer -> tail];
    buffer -> data[buffer -> tail] = 0;
    buffer -> tail = (buffer -> tail + 1) % BUFFER_SIZE;
    buffer -> size--;
    return c;
} 


/* ***********Funzioni richieste dal professore ********** 
char getChar(), that reads, if available a character from the input buffer. If the buffer is empty the "thread" asking for the character is put in a waiting queue. When a character becomes available the thread is brought back in running, and the character is returned (and consumed from the buffer)

void putChar(char), that writes (if there is enough space) a character on the output buffer. If the buffer is full, the thread is put in waiting, and resumed whenever there is enough room in the buffer.
*/

char getChar(void) {
    // Se il buffer di lettura è vuoto, metto il thread corrente nella coda di attesa di lettura
    if (inputBuffer.size == 0) {
        schedule();
    }

    // Se c'è qualcosa nel buffer di lettura, leggo e ritorno il carattere nel buffer
    char c;
    ATOMIC_BLOCK(ATOMIC_FORCEON) {
        c = bufferRead(&inputBuffer);
    }
    return c;
}

void putChar(char c) {
    // Se il buffer di scrittura è pieno, metto il thread corrente nella coda di attesa di scrittura
    if (outputBuffer.size >= BUFFER_SIZE) {
        schedule();
    }

    // Se non è pieno il buffer di scrittura, scrivo il carattere
    ATOMIC_BLOCK(ATOMIC_FORCEON) {
        bufferWrite(&outputBuffer, c);
    }
    UCSR0B |= _BV(UDRIE0); // Abilito interrupt di trasmissione
    return;
}

void printString(char* s) {
    int l=strlen(s);
    for(int i=0; i<l; ++i, ++s) {
        putChar(*s);
    }  
}


/* ***********Funzioni per scegliere i prossimi task in esecuzione ********** */
// Se c'è almeno un carattere nel buffer di input e c'è almeno un task nella coda di attesa di lettura, metto in esecuzione il primo task della coda di lettura e ritorno 1, altrimenti 0
uint8_t checkInput(void) {
    if (inputBuffer.size > 0 && reading_queue.size > 0) {
        TCB* next_input = TCBList_dequeue(&reading_queue);
        next_input -> status = Running;
        current_tcb = next_input;
        return 1;
    }
    return 0;
}

// Se c'è almeno un carattere nel buffer di output e c'è almeno un task nella coda di attesa di scrittura, metto in esecuzione il primo task della coda di scrittura e ritorno 1, altrimenti 0
uint8_t checkOutput(void) {
    if (outputBuffer.size < BUFFER_SIZE && writing_queue.size > 0) {
        TCB* next_output = TCBList_dequeue(&writing_queue);
        next_output -> status = Running;
        current_tcb = next_output;
        return 1;
    }
    return 0;
}