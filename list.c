#include <stdio.h>
#include <stdlib.h>
#include "list.h"

/* Names: Ashley Jones, Laura Slade 
 * Project 1 - Updated for Project 4
 * Due 11/17/14
*/


/* your list function definitions */



void list_clear(struct node *list) {
    while (list != NULL) {
        struct node *tmp = list;
        list = list->next;
        free(tmp);
    }
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

void list_insert_second(struct node *curr, struct node *head) {
    curr->next = head->next;
    head->next = curr;
}


