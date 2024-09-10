#include "buf.h"
#include <stdio.h>


int main() {
    Buffer inputBuffer;
    bufferInit(&inputBuffer);

    // Writing some values
    bufferWrite(&inputBuffer, 'A');
    bufferWrite(&inputBuffer, 'B');
    bufferWrite(&inputBuffer, 'C');

    // Printing the buffer
    printBuffer(&inputBuffer);

    // Reading one value
    char ch = bufferRead(&inputBuffer);
    printf("Letto: %c\n", ch);

    // Printing again
    printBuffer(&inputBuffer);

    return 0;
}