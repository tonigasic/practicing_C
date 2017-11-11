/* 
*
* Napisati funkciju koja primi stablo stupnja 1000, te provjeri je li ono lista ili ne
* (stablo je lista ukoliko svaki čvor ima samo jedno dijete).
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node {
	int value;
	struct node *children[1000];
};

int is_tree_list(struct node *root) {
	if (!root)
		return 0;
	int i;
	struct node* temp = NULL;
	for (i = 0; i < 1000; ++i)
	{
		if (root->children[i]) {
			if (temp)
				return 0;
			temp = root->children[i];
		}
	}
	if (temp)
		return is_tree_list(temp);
	return 1;
	

}

/* test case */

void tree_free(struct node *root) {

	int i;

	if (root == NULL)
		return;

	for (i = 0; i < 1000; ++i)
		tree_free(root->children[i]);

	free(root);

}

int main() {

	struct node *node0, *node1, *node2;

	node0 = malloc(sizeof(struct node));
	node0->value = 0;
	memset(node0->children, 0, sizeof(node0->children));

	node1 = malloc(sizeof(struct node));
	node1->value = 1;
	memset(node1->children, 0, sizeof(node1->children));

	node2 = malloc(sizeof(struct node));
	node2->value = 1;
	memset(node2->children, 0, sizeof(node2->children));

	node0->children[0] = node1;
	node1->children[17] = node2;

	printf("Stablo %s lista\n", is_tree_list(node0) ? "JEST" : "NIJE");

	tree_free(node0);

	return 0;

}