#include <avr/interrupt.h>
#include <avr/io.h>
#include <assert.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>
#include <util/atomic.h>


#include "tcb.h"
#include "tcb_list.h"
#include "uart.h"
#include "atomport_asm.h"
#include "scheduler.h"
#include "my_functions/buf.h"
#include "my_functions/functions.h"
#define THREAD_STACK_SIZE 256
#define IDLE_STACK_SIZE 128

// Global variables
Buffer reading_buffer;
Buffer writing_buffer;

char output[BUFFER_SIZE];   

//statically allocated variables where we put our stuff

TCB idle_tcb;
uint8_t idle_stack[IDLE_STACK_SIZE];
void idle_fn(uint32_t thread_arg __attribute__((unused))){
  while(1) {
    // cli();
    
    ATOMIC_BLOCK(ATOMIC_FORCEON){
      printf("[idle] Waiting to receive a character\n");

      // Adding the character to the reading buffer
      char x = usart_getchar();
      bufferWrite(&reading_buffer, x);

      // Getting the character form the reading buffer and putting it in the writing buffer
      // x = getChar();
      // putChar(x);
    }
    
    // sei();
    _delay_ms(1000);
  }
}

TCB r1_tcb;
uint8_t r1_stack[THREAD_STACK_SIZE];
void r1_fn(uint32_t arg __attribute__((unused))){
  while(1){
    // cli();
    
    ATOMIC_BLOCK(ATOMIC_FORCEON) {
      printf("[r1] Printing reading_buffer\n");
      printBuffer(&reading_buffer);
    }
    
    
  //  sei();
  _delay_ms(1000);
  }
}

TCB w2_tcb;
uint8_t w2_stack[THREAD_STACK_SIZE];
void w2_fn(uint32_t arg __attribute__((unused))){
  while(1){
    // cli();

    ATOMIC_BLOCK(ATOMIC_FORCEON) {
      printf("[w2] Printing writing_buffer\n"); 
      printBuffer(&writing_buffer);
    }

    // sei();
    _delay_ms(1000);
  }
}



int main(void){
  // we need printf for debugging
  printf_init();
  enableRxInterrupt();

  // Initializing buffers
  bufferInit(&reading_buffer);
  bufferInit(&writing_buffer);
  
  TCB_create(&idle_tcb,
             idle_stack+IDLE_STACK_SIZE-1,
             idle_fn,
             0);

  TCB_create(&r1_tcb,
             r1_stack+THREAD_STACK_SIZE-1,
             r1_fn,
             0);

  TCB_create(&w2_tcb,
             w2_stack+THREAD_STACK_SIZE-1,
             w2_fn,
             0);

  
  TCBList_enqueue(&running_queue, &r1_tcb);
  TCBList_enqueue(&running_queue, &w2_tcb);
  TCBList_enqueue(&running_queue, &idle_tcb);

  printf("starting\n");
  startSchedule();
}
