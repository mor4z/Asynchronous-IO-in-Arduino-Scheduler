#include "functions.h"


int main() {
    RingBuffer buf;

    printf("************* bufferInit() test *************\n");
    bufferInit(&buf);
    bufferInfo(&buf);
    printf("===================================================================\n\n");

    char x = 'a', y = 'c', z = 'p';

    printf("************* bufferWrite() test *************\n");
    bufferWrite(&buf, x);
    bufferWrite(&buf, y);
    bufferWrite(&buf, z);
    bufferWrite(&buf, y);
    for (int i = 0; i < 66; i++) {
        bufferWrite(&buf, y);
    }
    bufferInfo(&buf);
    printf("===================================================================\n\n");

    printf("************* bufferRead() test *************\n");
    char t;
    printf("Caratter letti: ");
    for (int j = 0; j < 66; j++) {
        t = bufferRead(&buf);
        printf("%c", t);
    }
    printf("\n");
    bufferInfo(&buf);
    printf("===================================================================\n\n");
}