#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "entry.h"
#include "utils/utils.h"

enum COLOR {
	RED,
	BLACK,
};

struct Node {
	struct Node *parent;
	struct Node *left_kid;
	struct Node *right_kid;
	struct Entry entry;
	enum COLOR color;
	int nodes_in_left_subtree;
};

struct Node *create_null_node(void);
struct Node *create_node(struct Entry e, struct Node *null, enum COLOR color);
void free_node(struct Node *node);
struct Node *grand_parent(struct Node *node);
struct Node *brother(struct Node *node);
struct Node *uncle(struct Node *node);
/*
 * return 1 if it node is  right_kid and 0 if it is a left_kid
 */
int is_right_kid(struct Node *node);
void replace_self_in_parent(struct Node *self, struct Node *replacement,
		struct Node *null);

struct RedBlackTree {
	struct Node *null;
	struct Node *root;
	int line_no;
};

struct RedBlackTree *create_RBTree(void);
void rise(struct RedBlackTree *rbtree, struct Node *x);
void insert_fixup(struct RedBlackTree *rbtree, struct Node *problem);
int insert_value(struct RedBlackTree *rbtree, struct Entry entry);
struct Node *minimum_node(struct RedBlackTree *rbtree, struct Node *source);
struct Node *maximum_node(struct RedBlackTree *rbtree, struct Node *source);
void remove_fixup(struct RedBlackTree *rbtree, struct Node *double_black);
void remove_node(struct RedBlackTree *rbtree, struct Node *to_remove);
void remove_value(struct RedBlackTree *rbtree, struct Entry entry);
struct Entry minimum_entry(struct RedBlackTree *rbtree);
struct Entry maximum_entry(struct RedBlackTree *rbtree);
void deleter_node(struct RedBlackTree *rbtree, struct Node *source);
void delete_tree(struct RedBlackTree *rbtree);
int is_empty(struct RedBlackTree *rbtree);
void print(struct RedBlackTree *rbtree);
void println(struct RedBlackTree *rbtree);
void println_tabbed(struct RedBlackTree *rbtree);
void printr_node(struct Node *source, struct Node *null, int andancime);
void printr_node_tabbed(struct Node *source, struct Node *null, int andancime);
