/* Ashley Jones
 * Laura Slade
 * Due: November 17, 2014
 * With thanks to Professor Stratton for helping us understand Stage 1 
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <strings.h>
#include <string.h>

#include "threadsalive.h"
#include "list.h"


//global variable: head of queue - pointer to a context
ucontext_t *head = NULL;
//global variable: tail of queue - pointer to a context but it's NULL
ucontext_t *tail = NULL;

/* ***************************** 
     stage 1 library functions
   ***************************** */

void ta_libinit(void) { //assume that whatever calls this is the main thread
//set up everything and create the first context for the main thread
//context for the FIRST thread aka main thread
//no stack for main thread - already has stack in system
//getcontext() function -- take all the context for current thread in operating system and dump it here (this makes the main thread): copy from the operating sytem from OS into your context buffers
//initialize fields using getcontext
//all you need for main is getcontext
//context saves the thread - when a thread goes to sleep, this is where all the info goes for when it needs to be woken up again
//creating a place to put a thread to sleep some time later
    //run queue of threads to be run - initialize run queue: one thread main
//allocate new context, initialze context using getcontext; front of queue is this main thread
//ucontext structure for main - initialize run queue with run
    ucontext_t ctx[1];
    getcontext(&ctx[0]);
    ctx[0].uc_link = NULL;
    head = &ctx[0];
    tail = &ctx[0];
    return;
}

void ta_create(void (*func)(void *), void *arg) {
    //create NEW threads
    //assume main thread and main thread context already exists
    //stick at end of run queue
    return;
}

void ta_yield(void) {
//current head node, sticks it on the end of the queue
//go to sleep
//stuff happens
//wake up
//after wake up, check why you woke up
//if woke up bc another thread yielded - you are all set (you should be the head)



//if woke up bc another thread returned (you will not be the head)
//then deallocated the current head and put head as next one


    return;
}

int ta_waitall(void) {
//wait all funciton cannot return until all the other functions finsih
//while i am not the only thread running, then have main sleep again
//swap context is basically yield

//run queue
//threads that are alive but can't be run right now (waiting)


//wake up: yielded or finished - how do you know yielded or finished?
//returned but not yielded, don't move head

//am i the only thing in the queue? no? yield (while loop)

    return -1;
}


/* ***************************** 
     stage 2 library functions
   ***************************** */

void ta_sem_init(tasem_t *sem, int value) {
}

void ta_sem_destroy(tasem_t *sem) {
}

void ta_sem_post(tasem_t *sem) {
}

void ta_sem_wait(tasem_t *sem) {
}

void ta_lock_init(talock_t *mutex) {
}

void ta_lock_destroy(talock_t *mutex) {
}

void ta_lock(talock_t *mutex) {
}

void ta_unlock(talock_t *mutex) {
}


/* ***************************** 
     stage 3 library functions
   ***************************** */

void ta_cond_init(tacond_t *cond) {
}

void ta_cond_destroy(tacond_t *cond) {
}

void ta_wait(talock_t *mutex, tacond_t *cond) {
}

void ta_signal(tacond_t *cond) {
}

