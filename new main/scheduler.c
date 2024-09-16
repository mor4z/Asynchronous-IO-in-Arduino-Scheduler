#include <avr/interrupt.h>
#include <avr/io.h>
#include <assert.h>
#include "tcb.h"
#include "tcb_list.h"
#include "atomport_asm.h"
#include "timer.h"
#include "buf.h"

// the (detached) running process
TCB* current_tcb=NULL;

// the running queue
TCBList running_queue={
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
  // we put back the current thread in the queue
  TCBList_enqueue(&running_queue, current_tcb);

  // we fetch the next;
  current_tcb=TCBList_dequeue(&running_queue);
  // we jump to it (useless if it is the only process)
  if (old_tcb!=current_tcb)
    archContextSwitch(old_tcb, current_tcb);
}


ISR(USART0_RX_vect){
  // reading the caracter
  char c = UDR0;

  // changing the '\r' or '\n'  with the string terminator
  if(c == '\r' || c == '\n') {
    c = '\0';
  }

  // saving the character in the reading buffer
  bufferWrite(&reading_buffer, c);

  schedule();
}
