/* Implementacija sucelja ATP skupa binarnim stablom pretrazivanja. */

#include <stdio.h>
#include <stdlib.h>
#include "set.h"

struct node {
	set_element_t elem;
	struct node *parent;
	struct node *left;
	struct node *right;
};

struct set_tag {
	struct node *root;
	int size;
	int (*compare)(const set_element_t, const set_element_t);
};

struct set_iterator_tag {
	set_t set;
	struct node *current;
};

static int compare_simple(const set_element_t elem1, const set_element_t elem2);
static void set_add_all(set_t set, set_t elements_set);

static void bst_free(struct node *root);
static struct node **bst_find(struct node **root, set_element_t key, int (*compare)(const set_element_t, const set_element_t));
static int bst_add(struct node **root, set_element_t elem, int (*compare)(const set_element_t, const set_element_t));
static void bst_remove(struct node **root, set_element_t key, int (*compare)(const set_element_t, const set_element_t));
static void bst_remove_node(struct node **node);
static struct node *bst_inorder_successor(struct node *node);
static struct node *bst_min(struct node *node);
static int tree_height(struct node *root);

/* public: ATP interface */

/* O(1) */
set_t set_new() {
	return set_new_advanced(compare_simple, NULL);
}

/* O(1) */
set_t set_new_advanced(int (*compare)(const set_element_t, const set_element_t), size_t (*hash)(const set_element_t)) {
	set_t set = malloc(sizeof(struct set_tag));
	set->root = NULL;
	set->size = 0;
	set->compare = compare;
	return set;
}

/* O(log n) - O(n), prosjecno O(sqrt n) */
void set_delete(set_t set) {
	bst_free(set->root);
	free(set);
}

/* O(1) */
int set_is_empty(set_t set) {
	return set->size == 0;
}

/* O(1) */
int set_size(set_t set) {
	return set->size;
}

/* O(log n) - O(n), prosjecno O(sqrt n) */
void set_add(set_t set, set_element_t elem) {
	set->size += bst_add(&set->root, elem, set->compare);
}

/* O(log n) - O(n), prosjecno O(sqrt n) */
void set_remove(set_t set, set_element_t elem) {
	bst_remove(&set->root, elem, set->compare);
	set->size--;
}

/* O(log n) - O(n), prosjecno O(sqrt n) */
int set_is_member(set_t set, set_element_t elem) {
	return bst_find(&set->root, elem, set->compare) != NULL;
}

/* O(nlog n) - O(n^2), prosjecno O(n) */
int set_is_subset(set_t set1, set_t set2) {
	set_iterator_t iter = set_iterator(set1);
	while (set_iterator_is_valid(iter)) {
		if (!set_is_member(set2, set_iget(iter)))
			return 0;
		else
			set_iterator_next(iter);
	}
	set_iterator_delete(iter);
	return 1;
}

/* O(nlog n) - O(n^2), prosjecno O(n) */
set_t set_union(set_t set1, set_t set2) {
	set_t union_set = set_new();
	set_add_all(union_set, set1);
	set_add_all(union_set, set2);
	return union_set;
}

/* O(nlog n) - O(n^2), prosjecno O(n) */
set_t set_intersection(set_t set1, set_t set2) {
	set_t intersection_set;
	set_iterator_t iter;
	intersection_set = set_new();
	iter = set_iterator(set1);
	while (set_iterator_is_valid(iter)) {
		set_element_t elem = set_iget(iter);
		if (set_is_member(set2, elem))
			set_add(intersection_set, elem);
		set_iterator_next(iter);
	}
	set_iterator_delete(iter);
	return intersection_set;
}

/* O(nlog n) - O(n^2), prosjecno O(n) */
set_t set_difference(set_t set1, set_t set2) {
	set_t difference_set;
	set_iterator_t iter;
	difference_set = set_new();
	iter = set_iterator(set1);
	while (set_iterator_is_valid(iter)) {
		set_element_t elem = set_iget(iter);
		if (!set_is_member(set2, elem))
			set_add(difference_set, elem);
		set_iterator_next(iter);
	}
	set_iterator_delete(iter);
	return difference_set;
}

/* O(log n) - O(n), prosjecno O(sqrt n) */
set_iterator_t set_iterator(set_t set) {
	set_iterator_t iter = malloc(sizeof(struct set_iterator_tag));
	iter->set = set;
	iter->current = set->root != NULL ? bst_min(set->root) : NULL;
	return iter;
}

/* O(1) */
set_element_t set_iget(set_iterator_t iter) {
	return iter->current->elem;
}

/* O(log n) - O(n), prosjecno O(sqrt n) */
void set_iremove(set_iterator_t iter) {
	int update_root = iter->set->root == iter->current;
	bst_remove_node(&iter->current);
	if (update_root)
		iter->set->root = iter->current;
	iter->set->size--;
}

/* O(log n) - O(n), prosjecno O(sqrt n) */
void set_iterator_next(set_iterator_t iter) {
	iter->current = bst_inorder_successor(iter->current);
}

/* O(1) */
int set_iterator_is_valid(set_iterator_t iter) {
	return iter->current != NULL;
}

/* O(1) */
int set_iterator_is_equal(set_iterator_t iter1, set_iterator_t iter2) {
	return iter1->current == iter2->current;
}

/* O(1) */
void set_iterator_set(set_iterator_t iter1, set_iterator_t iter2) {
	iter1->current = iter2->current;
}

/* O(1) */
void set_iterator_delete(set_iterator_t iter) {
	free(iter);
}

void set_debug_print_stats(set_t set) {
	printf("TREE SET STATS\n");
	printf("--------------\n");
	printf("Size: %d\n", set_size(set));
	printf("Tree height: %d\n", tree_height(set->root));
}

/* private: internals */

/* private: internals */

static int compare_simple(const set_element_t elem1, const set_element_t elem2) {
	if (elem1 > elem2)
		return 1;
	else if (elem1 < elem2)
		return -1;
	else
		return 0;
}

static void set_add_all(set_t set, set_t elements_set) {
	set_iterator_t iter = set_iterator(elements_set);
	while (set_iterator_is_valid(iter)) {
		set_add(set, set_iget(iter));
		set_iterator_next(iter);
	}
	set_iterator_delete(iter);
}

static void bst_free(struct node *root) {

	if (root == NULL)
		return;

	bst_free(root->left);
	bst_free(root->right);
	free(root);

}

static struct node **bst_find(struct node **root, set_element_t elem, int (*compare)(const set_element_t, const set_element_t)) {

	while (*root != NULL)
		if (compare((*root)->elem, elem) > 0)
			root = &(*root)->left;
		else if (compare((*root)->elem, elem) < 0)
			root = &(*root)->right;
		else
			return root;

	return NULL;

}

/* returns 1 if new element was allocated, 0 is existing was modified */
static int bst_add(struct node **root, set_element_t elem, int (*compare)(const set_element_t, const set_element_t)) {

	struct node *parent = NULL;

	while (*root != NULL)
		if (compare((*root)->elem, elem) > 0) {
			parent = *root;
			root = &(*root)->left;
		} else if (compare((*root)->elem, elem) < 0) {
			parent = *root;
			root = &(*root)->right;
		} else {
			return 0;
		}

	*root = malloc(sizeof(struct node));
	(*root)->elem = elem;
	(*root)->parent = parent;
	(*root)->left = NULL;
	(*root)->right = NULL;

	return 1;

}

static void bst_remove(struct node **root, set_element_t elem, int (*compare)(const set_element_t, const set_element_t)) {
	if (compare((*root)->elem, elem) == 0) {
		bst_remove_node(root);
	} else {
		struct node **node = bst_find(root, elem, compare);
		bst_remove_node(node);
	}
}

static void bst_remove_node(struct node **node) {

	if ((*node)->left == NULL) {

		struct node *old_node = *node;

		*node = (*node)->right;
		if (*node != NULL)
			(*node)->parent = old_node->parent;

		free(old_node);

	} else if ((*node)->right == NULL) {

		struct node *old_node = *node;

		*node = (*node)->left;
		if (*node != NULL)
			(*node)->parent = old_node->parent;

		free(old_node);

	} else {

		/* delete node with two children */
		set_element_t temp;

		/* find inorder predecessor */
		struct node **pred = &(*node)->left;
		while ((*pred)->right != NULL)
			pred = &(*pred)->right;

		/* swap values */
		temp = (*pred)->elem;
		(*pred)->elem = (*node)->elem;
		(*node)->elem = temp;

		bst_remove_node(pred);

	}

}

static struct node *bst_inorder_successor(struct node *node) {
	if(node->right != NULL) {
		return bst_min(node->right);
	} else {
		struct node *parent = node->parent;
		while(parent != NULL && node == parent->right) {
			node = parent;
			parent = parent->parent;
		}
		return parent;
	}
}

static struct node *bst_min(struct node *node) {
	struct node *current = node;
	while (current->left != NULL)
		current = current->left;
	return current;
}

static int tree_height(struct node *root) {

	int height_l, height_r;

	if (root == NULL)
		return 0;

	height_l = tree_height(root->left);		/* racunaj visinu lijevog podstabla */
	height_r = tree_height(root->right);	/* racunaj visinu desnog podstabla */

	/* visina trenutnog (pod)stabla je veca od visine lijevog odnosno desnog podstabla
	 * uvecano za 1 tj. visinu trenutnog cvora */
	return 1 + (height_l > height_r ? height_l : height_r);

}
