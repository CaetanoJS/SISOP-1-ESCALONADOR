#include <stdlib.h>
#include "caux.h"

int dispatcher() {

    TCB_t *READY_thread = retrieveReadyThread();
    TCB_t *EXEC_thread = g_scheduler.exec_thread;

    if (READY_thread == NULL)
        return -1;

    addToExec(READY_thread);
    swapcontext(&EXEC_thread->context, &READY_thread->context);

    return 0;
}

int createSchedulerContext(){
    char stack_dispatcher[SIGSTKSZ];
    if (getcontext(&scheduler_context) == RETURN_OK){
        scheduler_context.uc_stack.ss_sp = stack_dispatcher;
        scheduler_context.uc_stack.ss_size = sizeof(stack_dispatcher);
        makecontext( &scheduler_context, (void (*)(void))dispatcher, 0);
        return RETURN_OK;
    }
    return RETURN_ERROR;
}

void initQueues(){
    ready_queue = malloc(sizeof(FILA2));
    blocked_queue = malloc(sizeof(FILA2));
    finished_queue = malloc(sizeof(FILA2));
    ready_suspended_queue = malloc(sizeof(FILA2));
    blocked_suspended_queue = malloc(sizeof(FILA2));

    CreateFila2(ready_queue);
    CreateFila2(blocked_queue);
    CreateFila2(finished_queue);
    CreateFila2(ready_suspended_queue);
    CreateFila2(blocked_suspended_queue);
}

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

int initMainThread(){
    //main context stuff
    //char main_stack[SIGSTKSZ];  //STACK FOR MAIN CONTEXT
    TCB_t *main_thread = malloc(sizeof(TCB_t));

    if(main_thread != NULL){
        //init main TCB
        main_thread->tid = 0;
        main_thread->state = PROCST_EXEC;
        main_thread->prio = 0;
        if ((getcontext(&main_thread->context)) == RETURN_OK){
            return RETURN_OK;
        }
        return RETURN_ERROR;
    }
    return RETURN_ERROR;
}

int initialize(){

    //initialize all queues
    initQueues();

    //set the current thread to NULL
    current_thread = NULL;

    if (createSchedulerContext() == RETURN_OK){
        //init main thread
        if (initMainThread() == RETURN_OK){
            //set current thread to main
            current_thread = main_thread;
            has_init = 1;
            return RETURN_OK;
        }
    }
    return RETURN_ERROR;
}
