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
#include "functions.h" // Funzioni aggiunte
#define THREAD_STACK_SIZE 256
#define IDLE_STACK_SIZE 128


char rx[BUFFER_SIZE];  // Array di appoggio per stampare i caratteri ricevuti

// Processo di stampa: processo che dovrà stampare i caratteri dall'outputBuffer
TCB print_tcb;
uint8_t print_stack[IDLE_STACK_SIZE];
void print_fn(uint32_t thread_arg __attribute__((unused))){

    while(1) {
        cli();  // disabilita interrupt per leggere in sicurezza il buffer

        // Legge fino a PRINT_BUFFER_SIZE caratteri dal buffer di output
        int i = 0;
        while (i < BUFFER_SIZE - 1 && outputBuffer.size > 0) {
            rx[i++] = bufferRead(&outputBuffer);
        }

        sei();  // riabilita interrupt

        if (i > 0) {
            rx[i] = '\0';  // terminatore di stringa

            // Stampa sicura usando la funzione che scrive sul buffer TX
            // La ISR TX gestirà la scrittura reale su UDR0
            uint8_t* p = (uint8_t*)rx;
            while (*p) {
                putChar(*p++);
            }
        }

        // Piccola pausa per evitare busy wait continuo
        _delay_ms(50);
    }
}

// Altri processi: quando arriverà una interruzione dalla UART, questi dovranno scrivere sull'inputBuffer il carattere ricevuto getChar() e riportarlo sull'outputBuffer putChar(). Nel caso in cui non possano svolgere una di queste due operazioni, verranno messi nella rispettiva coda di attesa. 
TCB p1_tcb;
uint8_t p1_stack[THREAD_STACK_SIZE];
void p1_fn(uint32_t arg __attribute__((unused))){
  while(1){
    // printf("p1\n"); 
    
    cli();

    char c = getChar();
    putChar(c);

    sei();
    _delay_ms(100);
  }

}

TCB p2_tcb;
uint8_t p2_stack[THREAD_STACK_SIZE];
void p2_fn(uint32_t arg __attribute__((unused))){
  while(1){
    // printf("p2\n");
    
    cli();

    char c = getChar();
    putChar(c);

    sei();
    _delay_ms(100);
  }
}

TCB p3_tcb;
uint8_t p3_stack[THREAD_STACK_SIZE];
void p3_fn(uint32_t arg __attribute__((unused))){
  while(1){
    // printf("p3\n");
    
    cli();

    char c = getChar();
    putChar(c);

    sei();
    _delay_ms(100);
  }
}

TCB p4_tcb;
uint8_t p4_stack[THREAD_STACK_SIZE];
void p4_fn(uint32_t arg __attribute__((unused))){
  while(1){
    // printf("p4\n");
    
    cli();

    char c = getChar();
    putChar(c);

    sei();
    _delay_ms(100);
  }
}


int main(void){
  // we need printf for debugging
  printf_init();

  // Attivo gli interrupt in ricezione della seriale
  enableRxInterrupt();
  enableTxInterrupt();

  // Inizializzazione dei buffer
  bufferInit(&inputBuffer);
  bufferInit(&outputBuffer);  

  // Pulisco l'array di appoggio per stampare i caratteri ricevuti
  memset(&rx, 0, BUFFER_SIZE);

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
