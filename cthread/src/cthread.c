#include "../include/cdata.h"
#include "../include/support.h"
#include "../include/caux.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int has_init = 0;
int next_tid = 1;

TCB_t * current_thread; //thread atual
TCB_t *main_thread;
ucontext_t scheduler_context;

PFILA2 ready_queue;
PFILA2 blocked_queue;
PFILA2 finished_queue;
PFILA2 ready_suspended_queue;
PFILA2 blocked_suspended_queue;


// #################### CCREATE AND AUXILIARIES ###################

//INIT ALL QUEUES AND STRUCTURES FOR QUEUES
void initQueues(){
    ready_queue = malloc(sizeof(FILA2));
    blocked_queue = malloc(sizeof(FILA2));
    finished_queue = malloc(sizeof(FILA2));
    ready_suspended_queue = malloc(sizeof(FILA2));
    blocked_suspended_queue = malloc(sizeof(FILA2));

    if (CreateFila2(ready_queue) == RETURN_OK){
        printf("criou ready ok \n");
    }
    if (CreateFila2(blocked_queue) == RETURN_OK){
        printf("criou BLOCKED ok \n");
    }
    if (CreateFila2(finished_queue) == RETURN_OK){
        printf("criou finished_queue \n");
    }
    if (CreateFila2(ready_suspended_queue) == RETURN_OK){
        printf("ready suspend\n");
    }
    if (CreateFila2(blocked_suspended_queue) == RETURN_OK){
        printf("blocked suspend\n");
    }
}


//CREATE A NEW THREAD
TCB_t* createThread(){

    TCB_t * new_thread = malloc(sizeof(TCB_t));
    char stack[SIGSTKSZ];

    if(new_thread != NULL){
        new_thread->tid = next_tid;
        new_thread->state = PROCST_CRIACAO;
        new_thread->prio = 0;
        next_tid++;

        // new_thread->context.uc_stack.ss_sp = malloc(SIGSTKSZ);
        new_thread->context.uc_stack.ss_sp = stack;
        new_thread->context.uc_stack.ss_size = sizeof(stack);
        new_thread->context.uc_link = &scheduler_context;

        return new_thread;
    }
    return NULL;
}

//INITIALIZE MAIN THREAD
int initMainThread(){
    //main context stuff
    main_thread = malloc(sizeof(TCB_t));

    if(main_thread != NULL){
        //init main TCB
        main_thread->tid = 0;
        main_thread->state = PROCST_EXEC;
        main_thread->prio = 0;
        return getcontext(&main_thread->context);
    }

    return RETURN_ERROR;
}

//INITIALIZE ALL STRUCTURES
int initialize(){

    //initialize all queues
    initQueues();

    if (initMainThread() == RETURN_OK){
        current_thread = main_thread;
        has_init = 1;
        return RETURN_OK;
    }

    return RETURN_ERROR;
}


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
    TCB_t *new_thread;

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
// ########## END OF CCREATE AND AUXILIARIES ################


// ###### CIDENTIFY ######
int cidentify (char *name, int size){
    char id_grupo[IDENTIFY] = "Leonardo Eich 242314\nFelipe Fischer 242264\nCaetano Jaeger 242309\n";

	if (size >= IDENTIFY){
		strcpy(name, id_grupo);
		return RETURN_OK;
	}

	return RETURN_ERROR;
}
//## end of CIDENTIFY

int cyield(void){
    printf("executando cyield\n");
    //set the state to APTO
    current_thread->state = PROCST_APTO;

    //save the thread context
    if(getcontext(&current_thread->context) == RETURN_OK){
        printf("salvou contexto cyield\n");

        //put the current thread in the ready queue
        if(AppendFila2(ready_queue, current_thread) == RETURN_OK){
            printf("adicionou na fila de aptos\n");
            //call dispatcher
            dispatcher();
            return RETURN_OK;
        }
        printf("errou colocando na fila de apto\n");
        return RETURN_ERROR;
    }
    printf("errou salvando contexto \n");
    return RETURN_ERROR;
}
