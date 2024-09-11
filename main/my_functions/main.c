#include <stdio.h>

#include "buf.h"
#include "functions.h"

// Global variables
Buffer inputBuffer;
Buffer outputBuffer;


int main() {
    // Buffer inputBuffer;
    bufferInit(&inputBuffer);
    // Buffer outputBuffer;
    bufferInit(&outputBuffer);

    
    // Writing some values in the input buffer
    bufferWrite(&inputBuffer, '1');
    bufferWrite(&inputBuffer, '2');
    bufferWrite(&inputBuffer, '3');

    // Printing the input buffer
    printBuffer(&inputBuffer);

    // Reading one value from the input buffer and moving it to the output buffer
    char ch = bufferRead(&inputBuffer);
    printf("Letto: %c\n", ch);
    putChar(ch);

    // Reading another value from the input buffer and moving to the output buffer
    ch = bufferRead(&inputBuffer);
    printf("Letto: %c\n", ch);
    putChar(ch);

    // Reading another value from the input buffer and moving to the output buffer
    ch = bufferRead(&inputBuffer);
    printf("Letto: %c\n", ch);
    putChar(ch);

    // Printing the input buffer again
    printBuffer(&inputBuffer);

    // Printing the output buffer
    printBuffer(&outputBuffer);

    // Trying to read again from the input buffer, but I expect an error because it's empty
    ch = bufferRead(&inputBuffer);
    printf("Letto: %c\n", ch);

    // Printing the output buffer
    printBuffer(&outputBuffer);

    return 0;
}