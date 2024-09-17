#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>

#include "buf.h"
#include "functions.h"
#include "../scheduler.h"
#include "../tcb.h"
#include "../tcb_list.h"
#include "../atomport_asm.h"
#include "../uart.h"

// Activating RX interrupt
void enableRxInterrupt(){   
    UCSR0B |= (1<<RXCIE0);
}

char getChar(void) {
    if (reading_buffer.size == 0) {
        // Enqueuing the task in waiting queue
        printf("[getChar]Enqueuing the task in waiting queue\n");
        
        schedule();
    }

    // Return the char
    char ret = bufferRead(&reading_buffer);
    // if (ret != -1) {
    //     printf("Value %c read from the reading_buffer\n", ret);
    // }
    return ret;
}

void putChar(char data) {
    if (writing_buffer.size == BUFFER_SIZE) {
        // Enqueuing the task in waiting queue
        printf("[putChar]Enqueuing the task in waiting queue\n");
        
        schedule();
    }

    // writing the char on the output buffer
    bufferWrite(&writing_buffer, data);
    // printf("Value %c written in the writing_buffer \n", data);
}