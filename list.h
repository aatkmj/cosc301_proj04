#ifndef __LIST_H__
#define __LIST_H__

/* your list data structure declarations */
struct node {
    ucontext_t context; 
    int id;
    struct node *next; 
};

/* your function declarations associated with the list */

void list_clear(struct node *list);
void list_print(const struct node *list);
void list_append(struct node *added,struct node **head, struct node **tail);
void list_insert_second(struct node *curr, struct node *head);


#endif // __LIST_H__
