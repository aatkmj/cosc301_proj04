#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include <ucontext.h>


/* Names: Ashley Jones, Laura Slade 
 * Project 1 - Updated for Project 4
 * Due 11/17/14
*/


/* your list function definitions */



int list_clear(struct node *list) {
    int count = 0;
    while (list != NULL) {
        struct node *tmp = list;
        list = list->next;
        free(tmp->context.uc_stack.ss_sp); //free the context
        free(tmp); //free the node
        //free(tmp);
        count++;
    }
    return count;
}

void list_print(const struct node *list) {
    //int i = 0;
    printf("*** List Contents BEGIN ***\n");
    while (list != NULL) {
        printf("%d\n", list->id);
        list = list->next;
    }
    printf("*** List Contents END ***\n");
}

void list_append(struct node *added, struct node **head, struct node **tail) {

/* This fucntion insterts a new link in the linked list in the neumerically sorted correct position */

    if(*head == NULL) {
 	added->next = *head;
        *head = added;
        *tail = added;
	return;
    }
   else {
       struct node *tmp = *tail;
       tmp->next = added;
       added->next = NULL;
       *tail = added;
/*
       (*tail)->next = added;
       added->next = NULL;
*/
   }
}

void list_insert_second(struct node *curr, struct node **head, struct node **tail) {
	struct node *h = *head;
	if (h->next != NULL) {
    	curr->next = h->next;
   	 	h->next = curr;
		*head = h;
       }
	else {
		h->next = curr;
		curr->next = NULL;
		*head = h;
	    *tail = curr;
		}
}


