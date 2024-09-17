#include "buf.h"
#include <stdio.h>
#include <util/atomic.h>

// Function that initializes the fields of the buffer
void bufferInit(Buffer* buffer) {
    buffer -> head = 0;
    buffer -> tail = 0;
    buffer -> size = 0;
}

// Function that writes on the buffer
void bufferWrite(Buffer* buffer, char data) {
    ATOMIC_BLOCK(ATOMIC_FORCEON){
        buffer -> data[buffer -> head] = data;
        buffer -> head = (buffer -> head + 1) % BUFFER_SIZE;
        buffer -> size++;
    }
}

// Function that reads from the buffer
char bufferRead(Buffer* buffer) {
    uint8_t ret;
    ATOMIC_BLOCK(ATOMIC_FORCEON) {
        ret = buffer -> data[buffer -> tail];
        buffer -> data[buffer -> tail] = 0;
        buffer -> tail = (buffer -> tail + 1) % BUFFER_SIZE;
        buffer -> size--;
    }
    return ret;
}

// Function that prints the buffer
void printBuffer(Buffer* buffer) {
    
    printf("##################################################\n");
    printf("[");
    for (int i = 0; i < buffer -> size; ++i){
        int index = (buffer -> tail + i) % BUFFER_SIZE;
        printf("%c ", buffer -> data[index]);
    }
    printf("]\n");
    printf("##################################################\n");
    
}
