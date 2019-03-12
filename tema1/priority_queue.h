#pragma once

#include "rbtree.h"

typedef struct RedBlackTree PriorityQueue;

PriorityQueue *create_priority_queue(void);
void delete_priority_queue(PriorityQueue *pqueue);
int insert(PriorityQueue *pqueue, char *value, int priority);
int insert_entry(PriorityQueue *pqueue, struct Entry entry);
char *top(PriorityQueue *pqueue);
void pop(PriorityQueue *pqueue);
