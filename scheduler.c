#include <avr/interrupt.h>
#include <avr/io.h>
#include <assert.h>
#include "tcb.h"
#include "tcb_list.h"
#include "atomport_asm.h"
#include "timer.h"
#include "functions.h"

// the (detached) running process
TCB* current_tcb=NULL;

// the read waiting queue
TCBList reading_queue={
  .first=NULL,
  .last=NULL,
  .size=0
};

// the write waiting queue
TCBList writing_queue={
  .first=NULL,
  .last=NULL,
  .size=0
};

void startSchedule(void){
  cli();
  current_tcb=TCBList_dequeue(&reading_queue);
  assert(current_tcb);
  timerStart();
  archFirstThreadRestore(current_tcb);
}

void schedule(void) {
  TCB* old_tcb = current_tcb;

  // Metto il task corrente nella giusta coda di attesa
  if (old_tcb -> type == PRINT) {
    old_tcb -> status = Waiting;
    TCBList_enqueue(&writing_queue, current_tcb);
  } else {
    old_tcb -> status = Waiting;
    TCBList_enqueue(&reading_queue, current_tcb);
  }
    
  // Scelgo da dove pescare il prossimo task
  if (!checkInput())
    checkOutput();

  // we fetch the next;
  // we jump to it (useless if it is the only process)
  if (old_tcb!=current_tcb)
    archContextSwitch(old_tcb, current_tcb);
}


// ISR per la ricezione da seriale
ISR(USART_RX_vect) {

  PORTB |= _BV(PB5); // debugging

  // Scrittura del carattere ricevuto nel buffer
  char c = UDR0;
  if (c == '\n' || c == '\r') {
    c = '\0'; // Sostituisco l'invio con il terminatore di stringa
  }

  bufferWrite(&inputBuffer, c);

  schedule();
}

// ISR per la trasmissione da seriale
ISR(USART_UDRE_vect) {
  // Se non c'è nulla nel buffer di scrittura, abilito gli interrupt di trasmissione, sennò mando un carattere dal buffer
  if (outputBuffer.size == 0) {
    UCSR0B &= ~_BV(UDRIE0);
  } else {
    char c = bufferRead(&outputBuffer);
    UDR0 = c;
  }
  PORTB &= ~(1 << PB5); // debugging

  schedule();
}
