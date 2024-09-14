#include "tcb.h"
#include "tcb_list.h"

// the (detached) running process
extern TCB* current_tcb;

// the running queue
extern TCBList running_queue;

// the waiting queue
extern TCBList reading_waiting_queue;
extern TCBList writing_waiting_queue;

// starting the scheduler
void startSchedule(void);

// used in the ISR of the timer
void schedule(void);