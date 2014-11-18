/* Ashley Jones
 * Laura Slade
 * Due: November 17, 2014
 * With thanks to Professor Stratton for helping us understand Stage 1 
 * All stages and code were done together
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
#include "list.c"

//use linked list - then link all back to main
//swap between the main thread and the head of the list
//no good to have global blocked
//only times you run waitall you are main thread
//user thread calls yield
//current thread that called yield should be the head, put on end
//swap between that one and new head of ready queue 
//when something finishes it goes to waitall - then free memory
//then grab next yield
//no loop in yield
//while some thread left on ready queue swap to
//main as a global - never on queue (not a node) (doesn't point to anything) - never make it or get it 
//libinit shoudln't do anything.....

//global int - each had a new id (to help debugging) - so have an int id in the node

//head of ready queue - either just ran or just finished - don't swap until yield 
//have just the context not a pointer; malloc node, get context, make context, link
//free stack AND free node (2 mallocs and 2 frees)

//global variable: head of runable queue - pointer to a node
struct node *head = NULL;
//global variable: tail of runable queue - pointer to a node
struct node *tail = NULL;
//global variable: context id counter (0 as main thread)
static int context_id = 1;
//global variable: main thread
static ucontext_t main_thread;


//global varaiable: semaphore id
static int sem_id = 0;
//global variable: killed threads from sem destroy
static int killed = 0;
//global variable: yield call came from sem wait (0 is no, 1 is yes)
static int came_from_sem = 0; 


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

   
    getcontext(&main_thread);
    //main_thread.uc_link = NULL;
/*
    head = &main_thread;
    tail = &main_thread;
    */

    return;
}

void ta_create(void (*func)(void *), void *arg) {
    //create NEW threads
    //assume main thread and main thread context already exists
    //stick at end of run queue

    struct node *new = malloc(sizeof(struct node));

    #define STACKSIZE 128000
    unsigned char *stack1 = (unsigned char *)malloc(STACKSIZE);
    assert(stack1);

     
    getcontext(&new->context);

    new->context.uc_stack.ss_sp = stack1;
    new->context.uc_stack.ss_size = STACKSIZE;
    new->context.uc_link = &main_thread;

    new->id = context_id;
    context_id ++;

//link and append to ready queue
    list_append (new, &head, &tail);
 
//make context for thread
    
    makecontext(&new->context, (void (*)(void))func, 1, arg); 
    
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
    /*tail->uc_link = head; //tail points to head
    tail = head; //tail is updated to head ::::tail->uc_link
    head = head->uc_link; //head updated to new head
    tail->uc_link = NULL; //tail points to null
    */

    /*
    ucontext_t *temp = head;
    temp->uc_link = NULL;
    head = head->uc_link;
    tail->uc_link = temp;
    tail = tail->uc_link;

    ucontext_t *curr = malloc(sizeof(tail));
    curr = tail;

//wakes up
    while(head != curr) { //in case multiple threads have finished

    }    
    */
    if(came_from_sem == 0) {
    //move current head to the tail and update head to the next in line
    	struct node *temp = head;
    	head = head->next; 
    	temp->next = NULL;
    	list_append(temp, &head, &tail);
    
    	swapcontext(&tail->context, &head->context); 
	}
	else {
		//struct node *temp = head;
		if (head != NULL) {
			if (head->next == NULL){
				return;
			}
			struct node *temp2 = head;
			ucontext_t swap_temp = head->context;	
			head = head->next;

			came_from_sem = 0; //reset to 0 before leave function 
			swapcontext(&swap_temp, &head->context);
			free(temp2->context.uc_stack.ss_sp); //free the context
			free(temp2); //free node		
		}
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
    /*
    while(head->uc_link != NULL) {
        ta_yield();
    }
    if(blocked_head != NULL) {
        return -1; //if anything on the blocked queue, return -1
    }
    return 0; //nothing on the blocked queue
    */
    swapcontext (&main_thread, &head->context);

    while(head->next != NULL){
        struct node *temp = head;
        head = head->next;
        free(temp->context.uc_stack.ss_sp); //free the context
        free(temp); //free the node
		
        swapcontext (&main_thread, &head->context);
    }
	if (head != NULL) {
	    struct node *temp = head;
        free(temp->context.uc_stack.ss_sp); //free the context
        free(temp); //free the node
		}
		
    if (killed != 0){
		return -1;
    }
    return 0;
}


/* ***************************** 
     stage 2 library functions
   ***************************** */
//mutex is a binary semaphore - 
//initialize it to one
//only one thread running at a time
//just add to a (local) blocked queue and yeild thread! (don't spin wait bc then you will spin forever)

//on blocked queue - next thing it should do:
//WAIT: while value == 0, add to blocked queue, yeild, value -= 1
//POST: value+=1; if blocked queue != empty, put head of blocked on ready queue (insert as the second node - to keep the currently running thread as the head)

void ta_sem_init(tasem_t *sem, int value) {
    //make blocked queue here?
    /*assert(value >= 0);
    sem->value = value;
    //ta_lock_init(&sem->lock);
    //set value of counter
    */
    
    sem->counter = value;
    sem->id = sem_id;
    sem_id++;
    sem->blocked_h = NULL;
    sem->blocked_t = NULL;

}

void ta_sem_destroy(tasem_t *sem) {
    killed += list_clear(sem->blocked_h); //make sure to also free stack for thread!
}

void ta_sem_post(tasem_t *sem) {
    sem->counter++;
    if(sem->blocked_h != NULL) {
        struct node *temp = sem->blocked_h;
		sem->blocked_h = sem->blocked_h->next;
        temp->next = NULL;
        
        list_insert_second(temp, &head, &tail);
    }
}

void ta_sem_wait(tasem_t *sem) {
    //check
   // if (head == NULL){
	//	return;
	//}
    while(sem->counter == 0) {
        struct node *temp = malloc(sizeof(struct node));
/*        
		temp = head;
		temp->next = NULL;
*/
		temp->context = head->context;
		temp->id = head->id;
		temp->next = NULL;
        list_append(temp, &sem->blocked_h, &sem->blocked_t);
//maybe what we need to do is have something in yield that says if came from ta_sem_wait vs from other thread (bc here it means that something was blocked and that you shouldn't add it to the end of the ready queue!)
        came_from_sem = 1; //set true 
        ta_yield();

    }

		
    //decrement
    sem->counter--;
}

void ta_lock_init(talock_t *mutex) {
    /*
    mutex->counter = 1;
    mutex->id = lock_id;
    lock_id ++;
    mutex->blocked_h = NULL;
    mutex->blocked_t = NULL;
    */
    
	ta_sem_init(&mutex->lock, 1); //initialize lock with semaphore
}

void ta_lock_destroy(talock_t *mutex) {
    //killed += list_clear(mutex->blocked_h);
    
	ta_sem_destroy(&mutex->lock);  
}

void ta_lock(talock_t *mutex) {
/*
//don't yield during critical section
    mutex->counter++;
    if(mutex->blocked_h != NULL) {
        struct node *temp = mutex->blocked_h;
        temp->next = NULL;
        mutex->blocked_h = mutex->blocked_h->next;
        list_insert_second(temp, head);
    }
*/
    ta_sem_wait(&mutex->lock);

}

void ta_unlock(talock_t *mutex) {
/*    
    //check
    while(mutex->counter == 0) {
        struct node *temp =head;
        temp->next = NULL;
        list_append(temp, &mutex->blocked_h, &mutex->blocked_t);
        ta_yield();
    }
    //decrement
    mutex->counter--;
*/
    ta_sem_post(&mutex->lock);
}


/* ***************************** 
     stage 3 library functions
   ***************************** */

void ta_cond_init(tacond_t *cond) {
	cond->var = 0;
	cond->cond_h = NULL;
	cond->cond_t = NULL;
	
}

void ta_cond_destroy(tacond_t *cond) {
	killed += list_clear(cond->cond_h);
}

void ta_wait(talock_t *mutex, tacond_t *cond) {
	while (cond->var ==0){
	  //put all threads on blocked queue when cond = false
 		struct node *temp = malloc(sizeof(struct node));
		temp->context = head->context;
		temp->id = head->id;
		temp->next = NULL;
        list_append(temp, &cond->cond_h, &cond->cond_t);
        came_from_sem = 1; //set true 
//implicitly release locks
		ta_unlock(mutex);  
        ta_yield();

	}   
	if (cond->var == 1){
		return;
	//allow threads to pass through to critical section. (don't need to do anything here threads will just fall on through to critical section.)
	}
}

void ta_signal(tacond_t *cond) {
	cond->var = 1;
	if(cond->cond_h != NULL) {
        struct node *temp = cond->cond_h;
		cond->cond_h = cond->cond_h->next;
        temp->next = NULL;
        
        list_insert_second(temp, &head, &tail);
    }
	
}

