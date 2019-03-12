#pragma once

#include "rbtree.h"

typedef struct RedBlackTree PriorityQueue;

PriorityQueue *create_priority_queue(void);
void delete_priority_queue(PriorityQueue *pqueue);
struct Node *insert(PriorityQueue *pqueue, char *value, int priority);
struct Node *insert_entry(PriorityQueue *pqueue, struct Entry entry);
char *top(PriorityQueue *pqueue);
void pop(PriorityQueue *pqueue);
