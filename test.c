#include <avr/interrupt.h>
#include <avr/io.h>
#include <assert.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "tcb.h"
#include "tcb_list.h"
#include "uart.h"
#include "atomport_asm.h"
#include "scheduler.h"
#define THREAD_STACK_SIZE 256
#define PRINT_STACK_SIZE 128

#include "buffer.h"

char printBuffer[BUFFER_SIZE];

//statically allocated variables where we put our stuff

TCB print_tcb;
uint8_t print_stack[PRINT_STACK_SIZE];
void idle_fn(uint32_t thread_arg __attribute__((unused))){
  while(1) {
    cli();

    // Copio il buffer di input dentro printBuffer per stamparlo
    char* temp = printBuffer;
    while(inputBuffer.size > 0) {
      *temp = bufferRead(&inputBuffer); 
      temp++;     
    }
    printf("%s\n", printBuffer);

    // TODO: manda notifica che il buffer di output non è più vuoto


    // Pulisco printBuffer
    for (int i = 0; i < BUFFER_SIZE; i++) {
      printBuffer[i] = 0;
    }

    sei();
    _delay_ms(1000);
  }
}

TCB p1_tcb;
uint8_t p1_stack[THREAD_STACK_SIZE];
void p1_fn(uint32_t arg __attribute__((unused))){
  while(1){
    cli();
    printf("p1\n");
    sei();
    _delay_ms(1000);
  }
}

TCB p2_tcb;
uint8_t p2_stack[THREAD_STACK_SIZE];
void p2_fn(uint32_t arg __attribute__((unused))){
  while(1){
    cli();
    printf("p2\n");
    sei();
    _delay_ms(1000);
  }
}


int main(void){
  // we need printf for debugging
  printf_init();

  // Inizializzazione dei buffer
  bufferInit(&inputBuffer);
  bufferInit(&outputBuffer);
  // Pulisco il buffer di stampa
  memset(&printBuffer, 0, BUFFER_SIZE);

  // Abilito gli interrupt dovuti alla ricezione
  // enableRxInterrupt();

  TCB_create(&print_tcb,
             print_stack+PRINT_STACK_SIZE-1,
             idle_fn,
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
  TCBList_enqueue(&running_queue, &print_tcb);

  printf("starting\n");
  startSchedule();
}
