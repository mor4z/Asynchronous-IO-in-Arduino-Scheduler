#include <avr/interrupt.h>
#include <avr/io.h>
#include <assert.h>
#include <util/delay.h>
#include <util/atomic.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "tcb.h"
#include "tcb_list.h"
#include "uart.h"
#include "atomport_asm.h"
#include "scheduler.h"
#include "functions.h"
#define THREAD_STACK_SIZE 256
#define IDLE_STACK_SIZE 128

char tx[BUFFER_SIZE];
char rx[BUFFER_SIZE];
uint8_t size;

//statically allocated variables where we put our stuff
TCB p1_tcb;
uint8_t p1_stack[THREAD_STACK_SIZE];

TCB p2_tcb;
uint8_t p2_stack[THREAD_STACK_SIZE];

TCB p3_tcb;
uint8_t p3_stack[THREAD_STACK_SIZE];

TCB p4_tcb;
uint8_t p4_stack[THREAD_STACK_SIZE];

void print_fn(uint32_t arg __attribute__((unused))){
  while(1){
    cli();

    sprintf(tx, "%s \n", rx);
    if (strlen(rx) > 1)
      printString(tx);
    
    // Resetto gli array di appoggio per la stampa
    memset(tx, 0, BUFFER_SIZE);
    memset(rx, 0, BUFFER_SIZE);
    size = 0;

    sei();
    _delay_ms(100);
  }
}

void read_fn(uint32_t arg __attribute__((unused))){
  while(1){
    cli();

    char c = getChar();
    rx[size] = c;
    size++;
    rx[size] = 0;
    
    sei();
    _delay_ms(100);
  }
}


int main(void){
  usart_init(MYUBRR);

  sei();
  enableRxInterrupt();

  bufferInit(&inputBuffer);
  bufferInit(&outputBuffer);

  // LED su pin 13 (PB7) per debugging
  DDRB |= (1 << PB7);
  PORTB &= ~(1 << PB7);

  size = 0;
  memset(tx, 0, BUFFER_SIZE);
  memset(rx, 0, BUFFER_SIZE);

  TCB_create(&p1_tcb,
             p1_stack+THREAD_STACK_SIZE-1,
             read_fn,
             0,
             READ);

  TCB_create(&p2_tcb,
             p2_stack+THREAD_STACK_SIZE-1,
             read_fn,
             0,
             READ);
             
  TCB_create(&p3_tcb,
             p3_stack+THREAD_STACK_SIZE-1,
             read_fn,
             0,
             READ);
 
  TCB_create(&p4_tcb,
             p4_stack+THREAD_STACK_SIZE-1,
             print_fn,
             0,
             PRINT);
  
  TCBList_enqueue(&reading_queue, &p1_tcb);
  TCBList_enqueue(&reading_queue, &p2_tcb);
  TCBList_enqueue(&reading_queue, &p3_tcb);
  TCBList_enqueue(&writing_queue, &p4_tcb);

  usart_pstr("starting\n");
  startSchedule();
}
