#include <avr/interrupt.h>
#include <avr/io.h>
#include <assert.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>
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
Buffer inputBuffer;
Buffer outputBuffer;

char output[BUFFER_SIZE];   

//statically allocated variables where we put our stuff

TCB main_tcb;
uint8_t main_stack[IDLE_STACK_SIZE];
void main_fn(uint32_t thread_arg __attribute__((unused))){
  while(1) {
    cli();
    printf("main\n");
    // TODO: function that prints the characters in the output buffer, then cleans it 
    // before the next call to main_fn()
    sei();
    _delay_ms(1000);
  }
}

TCB p1_tcb;
uint8_t p1_stack[THREAD_STACK_SIZE];
void p1_fn(uint32_t arg __attribute__((unused))){
  while(1){
    cli();
    printf("[p1] Starting\n");

    // I read the character from the input buffer
    char c = getChar();

    // I move the character to the output buffer
    putChar(c);

    // Print for debugging
    printf("[p1] Character read: %c \n", c);
    printf("[p1] Ending\n");
    
    sei();
    _delay_ms(1000);
  }
}

TCB p2_tcb;
uint8_t p2_stack[THREAD_STACK_SIZE];
void p2_fn(uint32_t arg __attribute__((unused))){
  while(1){
    cli();
    printf("[p2] Starting\n");

    // I read the character from the input buffer
    char c = getChar();

    // I move the character to the output buffer
    putChar(c);

    // Print for debugging
    printf("[p2] Character read: %c \n", c);
    printf("[p2] Ending\n");
    sei();
    _delay_ms(1000);
  }
}



int main(void){
  // we need printf for debugging
  printf_init();

  TCB_create(&main_tcb,
             main_stack+IDLE_STACK_SIZE-1,
             main_fn,
             0);

  TCB_create(&p1_tcb,
             p1_stack+THREAD_STACK_SIZE-1,
             p1_fn,
             0);

  TCB_create(&p2_tcb,
             p2_stack+THREAD_STACK_SIZE-1,
             p2_fn,
             0);

  
  TCBList_enqueue(&running_queue, &p1_tcb);
  TCBList_enqueue(&running_queue, &p2_tcb);
  TCBList_enqueue(&running_queue, &main_tcb);

  printf("Starting the scheduler\n");
  startSchedule();
}
