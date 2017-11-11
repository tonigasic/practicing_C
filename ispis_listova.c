/* 
*
* Definirajte stablo stupnja 3, te napišite funkciju koja primi pokazivač na njegov korijen,
* te ispiše samo njegove listove (poredak nije bitan). */

#include <stdio.h>
#include <stdlib.h>

struct node {
	int value;
	struct node *left;
	struct node *middle;
	struct node *right;
};



/* test case */

static struct node *tree_add(struct node **root, int value) {

	if (*root != NULL)
		exit(-1);

	*root = malloc(sizeof(struct node));
	(*root)->value = value;
	(*root)->left = NULL;
	(*root)->middle = NULL;
	(*root)->right = NULL;

	return *root;

}

static void tree_free(struct node *root) {

	if (root == NULL)
		return;

	tree_free(root->left);
	tree_free(root->middle);
	tree_free(root->right);
	free(root);

}
void tree_print_leaves(struct node *root) {

	if (root == NULL)return;

	if (root->left == NULL&&root->middle == NULL&&root->right == NULL){
		printf("%d ", root->value);
		return;
	}
	tree_print_leaves(root->left);
	tree_print_leaves(root->middle);
	tree_print_leaves(root->right);

}

int main() {

	/*

	Izgled stabla:

	4
	_________/|\_________
	/          |          \
	3           0           7
	___/                    ___/ \___
	/                       /         \
	2                       5           9
	/ \                       \         /
	/   \                       \       /
	1    10                       6     8

	*/

	struct node *root = NULL;

	tree_add(&root, 4);
	{
		struct node *level1;
		level1 = tree_add(&root->left, 3);
		{
			struct node *level2;
			level2 = tree_add(&level1->left, 2);
			{
				tree_add(&level2->left, 1);
				tree_add(&level2->right, 10);
			}
		}
		level1 = tree_add(&root->middle, 0);
		level1 = tree_add(&root->right, 7);
		{
			struct node *level2;
			level2 = tree_add(&level1->left, 5);
			{
				tree_add(&level2->right, 6);
			}
			level2 = tree_add(&level1->right, 9);
			{
				tree_add(&level2->left, 8);
			}
		}
	}

	tree_print_leaves(root);

	tree_free(root);

	return 0;

}