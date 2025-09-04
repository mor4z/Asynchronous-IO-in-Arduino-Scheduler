#include <avr/interrupt.h>
#include <avr/io.h>
#include <assert.h>
#include "tcb.h"
#include "tcb_list.h"
#include "atomport_asm.h"
#include "timer.h"
#include "functions.h"

extern uint8_t uart_interrupt;

// the (detached) running process
TCB* current_tcb=NULL;

// Coda dei processi running
TCBList running_queue={
  .first=NULL,
  .last=NULL,
  .size=0
};

// Coda dei processi in attesa di lettura
TCBList reading_queue={
  .first=NULL,
  .last=NULL,
  .size=0
};

// Coda dei processi in attesa di scrittura
TCBList writing_queue={
  .first=NULL,
  .last=NULL,
  .size=0
};


void startSchedule(void){
  cli();
  current_tcb=TCBList_dequeue(&running_queue);
  assert(current_tcb);
  timerStart();
  archFirstThreadRestore(current_tcb);
}

void schedule(void) {
  TCB* old_tcb=current_tcb;
  // Rimetto il processo corrente nella coda di running
  TCBList_enqueue(&running_queue, current_tcb);

  // TODO: verifica se il buffer di input non è vuoto e se c'è un task nella lista di attesa di lettura per spostarlo nella cosa di ready
  checkInput();

  // TODO: verifica se il buffer di output non è pieno e se c'è un task nella lista di attesa di scrittura per spostarlo nella coda di ready
  checkOutput();
  
  // Rimuovo il processo corrente dalla coda di running
  current_tcb=TCBList_dequeue(&running_queue);
  // we jump to it (useless if it is the only process)
  if (old_tcb!=current_tcb)
    archContextSwitch(old_tcb, current_tcb);
}

// ISR per la ricezione da seriale
ISR(USART0_RX_vect) {
  cli();

  // Scrittura del carattere ricevuto nel buffer
  char c = UDR0;
  if (inputBuffer.size < BUFFER_SIZE) {
    bufferWrite(&inputBuffer, c);
  }

  // TODO: manda notifica che il buffer di input non è più vuoto
  checkInput();

  sei();
  schedule();
}

// ISR per la trasmissione da seriale
ISR(USART0_TX_vect) {
  cli();

  // Verifico se c'è ancora qualcosa da mandare
  if (outputBuffer.size > 0) {
    char c = bufferRead(&outputBuffer);
    UDR0 = c;
  } 

  checkOutput();

  sei();
  schedule();
}
