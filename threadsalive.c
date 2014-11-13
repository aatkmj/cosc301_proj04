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
#include <ucontext.h>
#include "threadsalive.h"
#include "list.h"


//global variable: head of runable queue - pointer to a context
static ucontext_t *head = NULL;
//global variable: tail of runable queue - pointer to a context but it's NULL
static ucontext_t *tail = NULL;

//global variable: head of blocked queue - pointer to a context
static ucontext_t *blocked_head = NULL;
//global variable: tail of blocked queue - pointer to a context but it's NULL
static ucontext_t *blocked_tail = NULL;

/* ***************************** 
     stage 1 library functions
   ***************************** */

void ta_libinit(void) { 
/*assume that whatever calls this is the main thread
set up everything and create the first context for the main thread
context for the FIRST thread aka main thread
no stack for main thread - already has stack in system
getcontext() function -- take all the context for current thread in operating system and dump it here (this makes the main thread): copy from the operating sytem from OS into your context buffers
initialize fields using getcontext
all you need for main is getcontext
context saves the thread - when a thread goes to sleep, this is where all the info goes for when it needs to be woken up again
creating a place to put a thread to sleep some time later
    run queue of threads to be run - initialize run queue: one thread main
allocate new context, initialze context using getcontext; front of queue is this main thread
ucontext structure for main - initialize run queue with run
*/

    ucontext_t main_thread;
    getcontext(&main_thread);
    main_thread.uc_link = NULL;
    head = &main_thread;
    tail = &main_thread;

    return;
}

void ta_create(void (*func)(void *), void *arg) {
    //create NEW threads
    //assume main thread and main thread context already exists
    //stick at end of run queue
    #define STACKSIZE 128
    unsigned char *stack1 = (unsigned char *)malloc(STACKSIZE);
    assert(stack1);
    ucontext_t ctx;
    getcontext(&ctx);
  
    printf("1/2 way through create....\n");

//link and append to queue
    tail->uc_link = &ctx;
    tail = &ctx;
 
//make context for thread
    makeconext(tail, (void (*)(void))func, arg);
    printf("make context done\n");
    return;
}

void ta_yield(void) {
//current head node, sticks it on the end of the queue
//go to sleep
//stuff happens
//wake up
//after wake up, check why you woke up
//wake up: yielded or finished - how do you know yielded or finished?
//returned but not yielded, don't move head
//if woke up bc another thread yielded - you are all set (you should be the head)

//if woke up bc another thread returned (you will not be the head)
//then deallocated the current head and put head as next one
    tail->uc_link = head; //tail points to head
    tail = tail->uc_link; //tail is updated to head
    head = head->uc_link; //head updated to new head
    tail->uc_link = NULL; //tail points to null

    ucontext_t *curr = malloc(sizeof(tail));
    curr = tail;
    
    swapcontext(tail, head); 

//wakes up
    while(head != curr) { //in case multiple threads have finished
        ucontext_t *temp = head;
        head = head->uc_link;
        free(temp);
    }    
    
    return;
}

int ta_waitall(void) {
//wait all funciton cannot return until all the other functions finsih
//while i am not the only thread running, then have main sleep again
//swap context is basically yield

//run queue
//threads that are alive but can't be run right now (waiting)

//am i the only thing in the queue? no? yield (while loop)

    while(head->uc_link != NULL) {
        ta_yield();
    }
    if(blocked_head != NULL) {
        return -1; //if anything on the blocked queue, return -1
    }
    return 0; //nothing on the blocked queue
  
 
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

