#include <stdio.h>

#include "buf.h"
#include "functions.h"
#include "../scheduler.h"
#include "../tcb.h"
#include "../tcb_list.h"
#include "../atomport_asm.h"
#include "../uart.h"

char getChar(void) {
    char ret = usart_getchar();

    if (reading_buffer.size == 0) {
        // Enqueuing the task in waiting queue
        printf("Enqueuing the task in waiting queue\n");
        return -1;
    }

    // Return the char
    ret = bufferRead(&reading_buffer);
    if (ret != -1) {
        printf("Value %c written in the reading_buffer\n", ret);
    }
    return ret;
}

void putChar(char data) {
    if (writing_buffer.size == BUFFER_SIZE) {
        // Enqueuing the task in waiting queue
        printf("Enqueuing the task in waiting queue\n");
    }

    // writing the char on the output buffer
    bufferWrite(&writing_buffer, data);
}