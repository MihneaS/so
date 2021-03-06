#include "rbtree.h"
#include "priority_queue.h"
#include "entry.h"
#include "utils/utils.h"

PriorityQueue *create_priority_queue(void)
{
	return create_RBTree();
}

void delete_priority_queue(PriorityQueue *pqueue)
{
	delete_tree(pqueue);
}
int insert(PriorityQueue *pqueue, char *value, int priority)
{
	struct Entry e = create_entry(priority, value, strlen(value));

	if (e.str == NULL)
		return ALLOC_FAIL;

	return insert_entry(pqueue, e);
}

int insert_entry(PriorityQueue *pqueue, struct Entry entry)
{
	return insert_value(pqueue, entry);
}

char *top(PriorityQueue *pqueue)
{
	if (!is_empty(pqueue))
		return maximum_entry(pqueue).str;
	else
		return NULL;
}

void pop(PriorityQueue *pqueue)
{
	if (!is_empty(pqueue))
		remove_node(pqueue, maximum_node(pqueue, pqueue->root));
}
