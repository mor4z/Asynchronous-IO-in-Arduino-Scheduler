#include <stdio.h>

#include "buf.h"
#include "functions.h"
#include "../scheduler.h"
#include "../tcb.h"
#include "../tcb_list.h"
#include "../atomport_asm.h"
#include "../uart.h"

char getChar(void) {
    char ret;

    if (inputBuffer.size == 0) {
        // Enqueuing the task in waiting queue
        printf("Enqueuing the task in waiting queue\n");
        return -1;
    }

    // Return the char
    ret = bufferRead(&inputBuffer);
    return ret;
}

void putChar(char data) {
    if (outputBuffer.size == BUFFER_SIZE) {
        // Enqueuing the task in waiting queue
        printf("Enqueuing the task in waiting queue\n");
    }

    // writing the char on the output buffer
    bufferWrite(&outputBuffer, data);
}