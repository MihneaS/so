#include "rbtree.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "entry.h"
#include "utils/utils.h"
#include "utils/compare.h"

struct Node *create_null_node(void)
{
	struct Node *ret = (struct Node *)malloc(sizeof(struct Node));

	if (ret == NULL)
		return NULL;

	ret->parent = NULL;
	ret->left_kid = NULL;
	ret->right_kid = NULL;
	ret->entry.key = 0;
	ret->entry.str = NULL;
	ret->color = BLACK;
	return ret;
}

struct Node *create_node(struct Entry e, struct Node *null, enum COLOR color)
{
	struct Node *ret = (struct Node *) malloc(sizeof(struct Node));

	if (ret == NULL)
		return NULL;

	ret->entry = e;
	ret->parent = null;
	ret->left_kid = null;
	ret->right_kid = null;
	ret->color = color;
	return ret;
}

void free_node(struct Node *node)
{
	free(node->entry.str);
	free(node);
}

struct Node *grand_parent(struct Node *node)
{
	return node->parent->parent;
}

struct Node *brother(struct Node *node)
{
	if (node->parent->left_kid == node)
		return node->parent->right_kid;
	else
		return node->parent->left_kid;
}

struct Node *uncle(struct Node *node)
{
	return brother(node->parent);
}

/*
 * return 1 if it node is  right_kid and 0 if it is a left_kid
 */
int is_right_kid(struct Node *node)
{
	return node->parent->right_kid == node;
}

void replace_self_in_parent(struct Node *self, struct Node *replacement,
		struct Node *null)
{
	if (replacement != null)
		replacement->parent = self->parent;
	if (self->parent->left_kid == self)
		self->parent->left_kid = replacement;
	else
		self->parent->right_kid = replacement;
}


struct RedBlackTree *create_RBTree(void)
{
	struct RedBlackTree *ret;
	struct Node *null = create_null_node();

	if (null == NULL)
		return NULL;

	ret = (struct RedBlackTree *)malloc(sizeof(struct RedBlackTree));
	if (ret == NULL)
		return NULL;
	ret->null = null;
	ret->root = ret->null;
	ret->root->parent = null;
	ret->root->left_kid = null;
	ret->root->right_kid = null;
	return ret;
}

/*
 *       p                            x
 *      / \    rise via x's right    / \
 *     x   o   ------------------>  l   p
 *    / \      aka right-rotate        / \
 *   l   r                            r   o
 *
 *       p                            x
 *      / \    rise via x's left     / \
 *     o   x   ------------------>  p   r
 *        / \  aka left-rotate     / \
 *       l   r                    o   l
 *
 *      l - x's original left
 *      r - x's original right
 *      p - x's original parent
 *      gp- x's original grandparent
 *      o - other
 *
 *      between l and r one will be stable (will retain its original place
 *      and the other one unstable, will become y's branch
 */

void rise(struct RedBlackTree *rbtree, struct Node *x)
{
	struct Node *p = x->parent;
	struct Node *gp = p->parent;

	/* legaturi gp - x*/
	if (p == rbtree->root) {
		rbtree->root = x;
		x->parent = rbtree->null;
	} else {
		if (gp->left_kid == p)
			gp->left_kid = x;
		else if (gp->right_kid == p)
			gp->right_kid = x;
		x->parent = gp;
	}
	/* legaturi x - p si p - unstable*/
	if (p->left_kid == x) {
		p->left_kid = x->right_kid;
		p->left_kid->parent = p;
		x->right_kid = p;
	} else {
		p->right_kid = x->left_kid;
		p->right_kid->parent = p;
		x->left_kid = p;
	}
	p->parent = x;
}

void insert_fixup(struct RedBlackTree *rbtree, struct Node *problem)
{
	struct Node *gp;

	while (problem != rbtree->root && problem->parent->color != BLACK) {
		gp = grand_parent(problem);
		if (uncle(problem)->color == RED) {
			gp->color = RED;
			gp->left_kid->color = BLACK;
			gp->right_kid->color = BLACK;
			problem = gp;
		} else {
			struct Node *last_problem;

			if (is_right_kid(problem) !=
					is_right_kid(problem->parent)) {
				rise(rbtree, problem);
				last_problem = problem;
			} else {
				last_problem = problem->parent;
			}
			rise(rbtree, last_problem);
			last_problem->color = BLACK;
			gp->color = RED; /* gp is now child of last problem*/
			return; /* job's done*/
		}
	}
	if (problem == rbtree->root)
		rbtree->root->color = BLACK;
}

int insert_value(struct RedBlackTree *rbtree, struct Entry e)
{
	struct Node *new_node = create_node(e, rbtree->null, RED);
	struct Node *father;
	struct Node *son = rbtree->root;

	if (new_node == NULL)
		return ALLOC_FAIL;

	while (son != rbtree->null) {
		father = son;
		if (compare(e.key, father->entry.key) < 0)
			son = father->left_kid;
		else
			son = father->right_kid;
	}
	if (son == rbtree->root) {
		new_node->color = BLACK;
		rbtree->root = new_node;
	} else {
		new_node->parent = father;
		if (compare(new_node->entry.key, father->entry.key) < 0)
			father->left_kid = new_node;
		else
			father->right_kid = new_node;
		insert_fixup(rbtree, new_node);
	}
	return 0;
}

struct Node *minimum_node(struct RedBlackTree *rbtree, struct Node *source)
{
	if (source != rbtree->null) {
		while (source->left_kid != rbtree->null)
			source = source->left_kid;
	}
	return source;
}

struct Node *maximum_node(struct RedBlackTree *rbtree, struct Node *source)
{
	if (source != rbtree->null) {
		while (source->right_kid != rbtree->null)
			source = source->right_kid;
	}
	return source;
}

void remove_fixup(struct RedBlackTree *rbtree, struct Node *double_black)
{
	struct Node *bro;
	struct Node *nephew_alike;
	struct Node *nephew_not_alike;

	while (double_black != rbtree->root && double_black->color == BLACK) {
		bro = brother(double_black);

		if (is_right_kid(double_black)) {
			nephew_alike = bro->right_kid;
			nephew_not_alike = bro->left_kid;
		} else {
			nephew_alike = bro->left_kid;
			nephew_not_alike = bro->right_kid;
		}

		if (bro->color == RED) {
			bro->color = BLACK;
			double_black->parent->color = RED;
			rise(rbtree, bro);
		} else if (bro->left_kid->color == BLACK
				&& bro->right_kid->color == BLACK) {
			bro->color = RED;
			double_black = double_black->parent;
		} else if (nephew_alike->color == RED
				&& nephew_not_alike->color == BLACK) {
			bro->color = RED;
			nephew_alike->color = BLACK;
			rise(rbtree, nephew_alike);
		} else /* if (nephew_not_alike->color == RED) */ {
			bro->color = bro->parent->color;
			bro->parent->color = BLACK;
			nephew_not_alike->color = BLACK;
			rise(rbtree, bro);
			break;
		}
	}
	double_black->color = BLACK;
}


/*	This function shall be called on leaves.
 *	It might DIE otherwise
 */
void remove_node(struct RedBlackTree *rbtree, struct Node *to_remove)
{
	if (to_remove == rbtree->null)
		return;
	if (to_remove == rbtree->root) {
		DIE(to_remove->right_kid != rbtree->null &&
				to_remove->right_kid != rbtree->null,
				"RedBlackTree:removing_node:wrong node");
		if (to_remove->right_kid == rbtree->null)
			rbtree->root = to_remove->left_kid;
		else
			rbtree->root = to_remove->right_kid;
		rbtree->root->color = BLACK;
	} else if (to_remove->left_kid == rbtree->null &&
			to_remove->right_kid == rbtree->null) {
		if (to_remove->color == BLACK)
			remove_fixup(rbtree, to_remove);
		replace_self_in_parent(to_remove,
				rbtree->null, rbtree->null);
	} else {
		if (to_remove->left_kid != rbtree->null) {
			replace_self_in_parent(to_remove,
					to_remove->left_kid, rbtree->null);
			to_remove->left_kid->color = BLACK;
		} else {
			replace_self_in_parent(to_remove,
					to_remove->right_kid, rbtree->null);
			to_remove->right_kid->color = BLACK;
		}
	}
	free_node(to_remove);
}

void remove_value(struct RedBlackTree *rbtree, struct Entry e)
{
	struct Node *n = rbtree->root;

	while (n != rbtree->null && compare(n->entry.key, e.key) != 0) {
		if (compare(e.key, n->entry.key) < 0)
			n = n->left_kid;
		else
			n = n->right_kid;
	}
	if (n != rbtree->null) {
		if (n->left_kid == rbtree->null ||
				n->right_kid == rbtree->null) {
			remove_node(rbtree, n);
		} else {
			struct Node *to_remove;

			to_remove = minimum_node(rbtree, n->right_kid);
			n->entry = to_remove->entry;
			remove_node(rbtree, to_remove);
		}
	}
}

struct Entry minimum_entry(struct RedBlackTree *rbtree)
{
	return minimum_node(rbtree, rbtree->root)->entry;
}

struct Entry maximum_entry(struct RedBlackTree *rbtree)
{
	return maximum_node(rbtree, rbtree->root)->entry;
}

void deleter_node(struct RedBlackTree *rbtree, struct Node *source)
{
	if (source == rbtree->null)
		return;
	deleter_node(rbtree, source->left_kid);
	deleter_node(rbtree, source->right_kid);
	free(source->entry.str);
	free(source);
}

void delete_tree(struct RedBlackTree *rbtree)
{
	deleter_node(rbtree, rbtree->root);
	free(rbtree->null);
	free(rbtree);
}

int is_empty(struct RedBlackTree *rbtree)
{
	return rbtree->root == rbtree->null;
}

void printr_node(struct Node *source, struct Node *null, int adancime)
{
	char color;

	if (source == null)
		return;
	if (source->color == BLACK)
		color = 'B';
	else
		color = 'R';
	printr_node(source->left_kid, null, adancime + 1);
	printf("{%ld:%s;%d:%c;%ld}",
			source->entry.key, source->entry.str,
			adancime, color, source->parent->entry.key);
	printr_node(source->right_kid, null, adancime + 1);
}

void printr_node_tabbed(struct Node *source, struct Node *null, int adancime)
{
	int i;
	char color;

	if (source == null)
		return;
	if (source->color == BLACK)
		color = 'B';
	else
		color = 'R';
	printr_node(source->left_kid, null, adancime + 1);
	for (i = 0; i < adancime; i++)
		printf("\t");
	printf("{%ld:%s;%d:%c;%ld\n}",
			source->entry.key, source->entry.str,
			adancime, color, source->parent->entry.key);
	printr_node(source->right_kid, null, adancime + 1);
}

void print(struct RedBlackTree *rbtree)
{
	printr_node(rbtree->root, rbtree->null, 0);
}


void println(struct RedBlackTree *rbtree)
{
	print(rbtree);
	printf("\n");
}
void println_tabbed(struct RedBlackTree *rbtree)
{
	printr_node_tabbed(rbtree->root, rbtree->null, 0);
}
