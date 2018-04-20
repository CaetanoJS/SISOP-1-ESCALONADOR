#include "../include/cdata.h"
#include "../include/support.h"
#include "../include/caux.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int has_init = 0;
int next_tid = 1;

TCB_t * current_thread; //thread atual
ucontext_t scheduler_context;

PFILA2 ready_queue;
PFILA2 blocked_queue;
PFILA2 finished_queue;
PFILA2 ready_suspended_queue;
PFILA2 blocked_suspended_queue;


/*
Creation of a thread: Allocation, management and initialization of the same.

Parameters:
Start: pointer to the function that the thread will execute.
Arg: a parameter that can be passed to the thread at its creation.
(Note: it is a single parameter. If it is necessary to pass more than one value a pointer to a struct must be used)
Priority: priority with which the thread must be created.
Return: Correct: returns a positive value representing the tid of the created thread
Error: When priority is invalid it returns -1
*/
int ccreate (void* (*start)(void*), void *arg, int prio){
    TCB_t *new_thread
    //if the main and the queues are note created yet, then create them
    if(!has_init){
        initialize();
    }

    if (getcontext(&scheduler_context) == RETURN_OK){
        //create new thread
        new_thread = createThread();
        makecontext(&new_thread->context, (void (*)(void))start, 1, arg);
        new_thread->state = PROCST_APTO;
        AppendFila2(ready_queue, new_thread);
        return new_thread->tid;
    }

    return RETURN_ERROR;

}

int cidentify (char *name, int size){
    char id_grupo[IDENTIFY] = "Leonardo Eich 242314\nFelipe Fischer 242264\nCaetano Jaeger 242309\n";

	if (size >= IDENTIFY){
		strcpy(name, id_grupo);
		return RETURN_OK;
	}

	return RETURN_ERROR;
}

int cyield(void){
    return 0;
}
