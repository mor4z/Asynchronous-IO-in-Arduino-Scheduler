#include "tcb.h"
#include "tcb_list.h"

// the (detached) running process
extern TCB* current_tcb;

extern TCBList running_queue; // Coda dei processi in esecuzione
extern TCBList reading_queue; // Coda dei processi in attesa di lettura
extern TCBList writing_queue; // Coda dei processi in attesa di scrittura

void startSchedule(void);

// used in the ISR of the timer
void schedule(void);
