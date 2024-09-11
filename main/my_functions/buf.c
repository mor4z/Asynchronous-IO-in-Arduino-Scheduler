#include "buf.h"
#include <stdio.h>



// Function that initializes the fields of the buffer
void bufferInit(Buffer* buffer) {
    buffer -> head = 0;
    buffer -> tail = 0;
    buffer -> size = 0;
}

// Function that writes on the buffer
void bufferWrite(Buffer* buffer, char data) {
    if (buffer -> size < BUFFER_SIZE) {
        buffer -> data[buffer -> head] = data;
        buffer -> head = (buffer -> head + 1) % BUFFER_SIZE;
        buffer -> size++;
    } else {
        printf("Buffer pieno\n");
    }
}

// Function that reads from the buffer
char bufferRead(Buffer* buffer) {
    if (buffer -> size > 0) {
        char ret = buffer -> data[buffer -> tail];
        buffer -> tail = (buffer -> tail + 1) % BUFFER_SIZE;
        buffer -> size--;
        return ret;
    }
    
    printf("Buffer vuoto\n");
    return -1;
}

// Function that prints the buffer
void printBuffer(Buffer* buffer) {
    printf("[");
    for (int i = 0; i < buffer -> size; ++i){
        int index = (buffer -> tail + i) % BUFFER_SIZE;
        printf("%c ", buffer -> data[index]);
    }
    printf("]\n");
}
