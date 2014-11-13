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
    int i = 0;
    printf("*** List Contents BEGIN ***\n");
    while (list != NULL) {
        printf("%d\n", list->value);
        list = list->next;
    }
    printf("*** List Contents END ***\n");
}

void list_insert(int value, struct node **head) {

/* This fucntion insterts a new link in the linked list in the neumerically sorted correct position */

    struct node *new = malloc(sizeof(struct node));
    new->value = value;

    if(*head == NULL || (**head).value > value) {
 	new->next = *head;
        *head = new;
	return;
    }
    // sort and find the right place 
    struct node *previous = *head;
    struct node *curr = previous->next;
    while(curr != NULL) { //if happens to be placed before the very end of the linked list, break
        if(curr->value > value) {
            new->next = curr;
            previous->next = new;
            return;
        }
	previous = curr;
	curr = curr->next;
    }
    previous->next = new;
    new->next = NULL;
}


