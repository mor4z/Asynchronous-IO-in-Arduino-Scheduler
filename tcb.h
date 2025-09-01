#pragma once
#include <stdint.h>
#include <stddef.h>

#define OK     0
#define ERROR -1

typedef uint8_t* Pointer;
typedef void (* ThreadFn)(uint32_t thread_args);

typedef enum {Running=0x0, Terminated=0x1, Ready=0x2, Waiting=0x3} ThreadStatus;     // States of the task when the program is working. Added status "Waiting" for the queue

// thread control block
typedef struct TCB {
  Pointer sp_save_ptr;          /* Where I save the stack pointer before the Context Switch*/
  
  ThreadFn thread_fn;           /* Function of the thread */
  uint32_t thread_arg;          /* Argument to be passed to the thread function until it starts*/

  struct TCB* next;
  struct TCB* prev;

  Pointer stack_bottom;         /* Pointer to bottom of stack allocation */
  uint32_t stack_size;          /* Size of stack allocation in bytes */
  ThreadStatus status;          /* Status of the thread taken from the enum above */
} TCB;  


void TCB_create(TCB* tcb, Pointer stack_top, ThreadFn thread_fn, uint32_t thread_arg);
