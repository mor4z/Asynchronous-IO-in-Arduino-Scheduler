#include "tcb_list.h"
#include <stdio.h>

// pops the first thread from the queue, and detaches it
TCB* TCBList_dequeue(TCBList* list){
  TCB* tcb=list->first;
  if (tcb==NULL)
    return tcb;
  
  if (list->size==1) {
    list->first = list->last = NULL;
  } else {
    TCB* next=tcb->next;
    // list->first=next;
    // next->prev=next;
    next -> prev = list -> first;
    list -> first = next;
  }
  --list->size;
  tcb->next=NULL;
  tcb->prev=NULL;
  return tcb;
}

// adds a thread to the end of the queue
// thread must be detached
uint8_t TCBList_enqueue(TCBList* list, TCB* tcb){
  if(tcb->prev!=NULL || tcb->next!=NULL)
    return ERROR;
  
  if (!list->size) {
    list->first=tcb;
    list->last=tcb;
    tcb->prev=tcb;
    tcb->next=NULL;
    tcb -> status = Waiting;
  } else {
    list->last->next=tcb;
    tcb->prev=list->last;
    tcb->next=NULL;
    list->last=tcb;
    tcb -> status = Waiting;
  }
  ++list->size;
  return OK;
}

