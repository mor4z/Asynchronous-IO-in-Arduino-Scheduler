#include <avr/interrupt.h>
#include <avr/io.h>
#include <assert.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <util/atomic.h>
#include "tcb.h"
#include "tcb_list.h"
#include "uart.h"
#include "atomport_asm.h"
#include "scheduler.h"
#include "functions.h" // Funzioni aggiunte
#define THREAD_STACK_SIZE 256
#define IDLE_STACK_SIZE 128

char tx[BUFFER_SIZE];
volatile uint8_t idx;

// Processo di stampa: processo che dovrà stampare i caratteri dall'outputBuffer
TCB print_tcb;
uint8_t print_stack[IDLE_STACK_SIZE];
void print_fn(uint32_t thread_arg __attribute__((unused))){
  idx = 0;
  while(1) {
    cli();
    
    ATOMIC_BLOCK(ATOMIC_FORCEON) {
      while (printBuffer.size > 0) {
        tx[idx] = bufferRead(&printBuffer);
        idx++;
      }
    }

    tx[idx] = '\0';

    if (strlen(tx) > 1)
      printf("%s\n", tx);

    // Notifico i processi nella coda di writing
    checkOutput();

    // Resetto l'array di appoggio per la stampa
    memset(tx, 0, BUFFER_SIZE);
    idx = 0;

    sei();  

    _delay_ms(100);
  }
}

// Altri processi: quando arriverà una interruzione dalla UART, questi dovranno scrivere sull'inputBuffer il carattere ricevuto getChar() e riportarlo sull'outputBuffer putChar(). Nel caso in cui non possano svolgere una di queste due operazioni, verranno messi nella rispettiva coda di attesa. 
TCB p1_tcb;
uint8_t p1_stack[THREAD_STACK_SIZE];
void p1_fn(uint32_t arg __attribute__((unused))){
  while(1){
    cli();
    // printf("p1: ");
    while (inputBuffer.size > 0) {
      char c = getChar();
      putChar(c);
    }
    
    sei();
    _delay_ms(100);
  }

}

TCB p2_tcb;
uint8_t p2_stack[THREAD_STACK_SIZE];
void p2_fn(uint32_t arg __attribute__((unused))){
  while(1){
    cli();
    // printf("p2: ");
    while (inputBuffer.size > 0) {
      char c = getChar();
      putChar(c);
    }

    sei();
    _delay_ms(100);
  }
}

TCB p3_tcb;
uint8_t p3_stack[THREAD_STACK_SIZE];
void p3_fn(uint32_t arg __attribute__((unused))){
  while(1){
    cli();
    // printf("p3: ");
    while (inputBuffer.size > 0) {
      char c = getChar();
      putChar(c);
    }

    sei();
    _delay_ms(100);
  }
}

TCB p4_tcb;
uint8_t p4_stack[THREAD_STACK_SIZE];
void p4_fn(uint32_t arg __attribute__((unused))){
  while(1){
    cli();
    // printf("p4: ");
    while (inputBuffer.size > 0) {
      char c = getChar();
      putChar(c);
    }

    sei();
    _delay_ms(100);
  }
}


int main(void){
  // we need printf for debugging
  printf_init();

  // Attivo le interruzioni globali
  sei();


  // Inizializzazione dei buffer
  bufferInit(&inputBuffer);
  bufferInit(&outputBuffer);  
  bufferInit(&printBuffer);

  // Uso il LED su pin 13 (PB7) per debugging (lo accendo quando arriva un interrupt in ricezione, lo spengo quando arriva un interrupt di fine trasmissione)
  DDRB |= (1 << PB7);
  PORTB &= ~(1 << PB7);
 
  // Creazione dei task 
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
  
  TCB_create(&p3_tcb,
             p3_stack+THREAD_STACK_SIZE-1,
             p3_fn,
             0);
 
  TCB_create(&p4_tcb,
             p4_stack+THREAD_STACK_SIZE-1,
             p4_fn,
             0);
 
  TCBList_enqueue(&running_queue, &p1_tcb);
  TCBList_enqueue(&running_queue, &p2_tcb);
  TCBList_enqueue(&running_queue, &p3_tcb);
  TCBList_enqueue(&running_queue, &p4_tcb);
  TCBList_enqueue(&running_queue, &print_tcb);

  printf("starting\n");
  startSchedule();
}
