#include "tcb.h"
#include "tcb_list.h"

// the (detached) running process
extern TCB* current_tcb;

void startSchedule(void);

// used in the ISR of the timer
void schedule(void);
     