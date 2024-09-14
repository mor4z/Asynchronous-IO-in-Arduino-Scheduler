#define BUFFER_SIZE 10

typedef struct {
    char data[BUFFER_SIZE];         // Pointer to the buffer array
    int head;                       // Index of the first element
    int tail;                       // Index of the last element
    int size;                       // Number of elements in the buffer
} Buffer;

void bufferInit(Buffer* buffer);

void bufferWrite(Buffer* buffer, char data);
char bufferRead(Buffer* buffer);
void printBuffer(Buffer* buffer);

// Buffers I need to use
extern Buffer reading_buffer;
extern Buffer writing_buffer;