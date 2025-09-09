#include "ringbuffer.h"
#include <stdio.h>
#include <util/atomic.h>

void bufferInit(RingBuffer* buffer) {
    buffer->head = 0;
    buffer->tail = 0;
    buffer->size = 0;
}

void bufferInfo(RingBuffer* buffer) {
    printf("Head: %d, Tail: %d, Size: %d\n", buffer->head, buffer->tail, buffer->size);
}

void bufferWrite(RingBuffer* buffer, char c) {
    if (buffer->size < BUFFER_SIZE) {
        buffer->data[buffer->head] = c;
        buffer->head = (buffer->head + 1) % BUFFER_SIZE;
        buffer->size++;
    }
}

char bufferRead(RingBuffer* buffer) {
    char c = 0;
    if (buffer->size > 0) {
        c = buffer->data[buffer->tail];
        buffer->tail = (buffer->tail + 1) % BUFFER_SIZE;
        buffer->size--;
    }
    return c;
}

char getChar(void) {
    while (inputBuffer.size == 0);  // attende un byte
    return bufferRead(&inputBuffer);
}

void putChar(char c) {
    // Scrive nel buffer di output
    ATOMIC_BLOCK(ATOMIC_FORCEON) {
        bufferWrite(&outputBuffer, c);
    }
    // Abilita interrupt di trasmissione
    UCSR0B |= _BV(UDRIE0);
}