#ifndef __LIST_H__
#define __LIST_H__

/* your list data structure declarations */
struct node {
    int value;
    struct node *next; 
};

/* your function declarations associated with the list */

void list_clear(struct node *list);
void list_print(const struct node *list);
void list_insert(int value, struct node **head);



#endif // __LIST_H__
