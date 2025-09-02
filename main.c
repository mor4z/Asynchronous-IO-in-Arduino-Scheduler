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
#include "buffer.h" // Funzioni del buffer
#define THREAD_STACK_SIZE 256
#define IDLE_STACK_SIZE 128



// Processo di stampa
TCB print_tcb;
uint8_t print_stack[IDLE_STACK_SIZE];
void print_fn(uint32_t thread_arg __attribute__((unused))){
  while(1) {
    cli();
    printf("print\n");
    sei();
    _delay_ms(1000);
  }
}

// Altri processi
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

  enableRxInterrupt();

  // Inizializzazione dei buffer
  bufferInit(&inputBuffer);
  bufferInit(&outputBuffer);  


  TCB_create(&print_tcb,
             print_stack+IDLE_STACK_SIZE-1,
             print_fn,
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
