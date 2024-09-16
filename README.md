# Asynchronous IO in Arduino Scheduler

Repository realized for the project from the course of Sistemi Operativi held by the teacher Giorgio Grisetti in university La Sapienza.

## A2. Asynchronous IO in Arduino Scheduler.
Add functions for buffered writing to the arduino preemptive scheduler.

The OS has two buffers, one for reading, one for writing.
Implement two functions:

* char getChar(), that reads, if available a character from the input buffer.
    If the buffer is empty the "thread" asking for the character
    is put in a waiting queue.
    When a character becomes available the thread is brought back in running,
    and the character is returned (and consumed from the buffer)

* void putChar(char), that writes (if there is enough space) a character on the output buffer.
    If the buffer is full, the thread is put in waiting, and resumed whenever there is enough room
    in the buffer.

## Directory 
In the directory main there are all the files I need to handle the task switch and all the structures used for the tasks. The subdirectory my_functions contains the functions that handle the input and output buffer and the two functions I have to implement.

In the directory serial there are all the files I need to use the serial of the Arduino to communicate between a reading process and a writing process.


## Working flow
Day 1 - 9/09/2024: I selected all useful files from the course repository and created the GitHub repository.

Day 2 - 10/09/2024: I started working on the input and output buffers, implementing all the functions to create and handle these buffers.

Day 3 - 11/09/2024: I started writing a prototype for the functions getChar() and putChar() and tested them with the functions for the buffer that I wrote on day 2.

Day 4 - 12/09/2024: I started creating the TCBList elements for the waiting queues, one for the input buffer and one for the output buffer. Then I started writing the thread functions for the tasks: the task 'main' is responsible for printing on the screen the next value of the output buffer; the tasks 'p1' and 'p2' are responsible for receiving the character and putting in first in the input buffer, then moving it to the output buffer.

Day 5 - 13/09/2024: After asking for some questions to the teacher, I realized that the workflow I have been following completely ignored the section of code where I handle the communication with the uart, that should be one of the first pieces of my program. So I freezed the main branch, and started working on a secondary branch, to recover the missing part.

Day 6 - 14/09/2024: I started working on the routine for the transmission and receive form the serial of the uart. I created a new directory called "new main" to work only on this part.

Day 7- 16/09/2024: I tested the transmission and faced a problem with the function printBuffer() that I wrote to print the values in a buffer. Asked the teacher a meeting to try to solve this problem.