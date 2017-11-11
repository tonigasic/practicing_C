/*
*
* Definirajte binarno stablo za pohranu vrijednosti tipa unsigned,
* te napišite funkciju koja primi dva stabla i vrati 1
* ukoliko su ona identična, a 0 inače.
* Za dva stabla kažemo da su identična ukoliko su jednake strukture,
* te u čvorovima na istim pozicijama sadrže iste vrijednosti. 
*/
#include <stdio.h>
#include <stdlib.h>

struct node {
	int value;
	struct node *left;
	struct node *right;
};

int tree_equal(struct node *root1, struct node *root2) {
	if (root1 == NULL || root2 == NULL)return 1;
	if (root1->value != root2->value) return 0;

	tree_equal(root1->left, root2->left);
	tree_equal(root1->right, root2->right);

	
	
}

/* test case */

struct node *bst_add_recursive(struct node **root, int value) {

	if (*root == NULL) {
		*root = malloc(sizeof(struct node));
		(*root)->value = value;
		(*root)->left = NULL;
		(*root)->right = NULL;
		return *root;
	}
	else if ((*root)->value > value)
		return bst_add_recursive(&(*root)->left, value);
	else if ((*root)->value < value)
		return bst_add_recursive(&(*root)->right, value);
	else
		/* duplikat: (*root)->value == value */
		return *root;

}

void tree_print_inorder(struct node *root) {

	if (root == NULL)
		return;

	tree_print_inorder(root->left);		/* inorder poretkom ispisi lijevo podstablo */
	printf("%d ", root->value);			/* ispisi trenutni cvor */
	tree_print_inorder(root->right);	/* inorder poretkom ispisi desno podstablo */

}

void tree_free(struct node *root) {

	if (root == NULL)
		return;

	tree_free(root->left);	/* oslobodi memoriju za sve cvorove iz lijevog podstabla */
	tree_free(root->right);	/* oslobodi memoriju za sve cvorove iz desnog podstabla */
	free(root);				/* oslobodi memoriju za trenutni cvor */

}

int main() {

	/*

	Izgled BST stabla:

	4
	_________/ \_________
	/                     \
	3                       7
	___/                    ___/ \___
	/                       /         \
	2                       5           9
	/                         \         /
	/                           \       /
	1                             6     8

	*/

	struct node *root1 = NULL;
	struct node *root2 = NULL;
	int data1[] = { 4, 7, 3, 5, 2, 9, 8, 6, 1 };
	int data2[] = { 4, 7, 3, 5, 2,9, 8, 6, 1 };
	int i;

	for (i = 0; i < sizeof(data1) / sizeof(*data1); ++i)
		bst_add_recursive(&root1, data1[i]);

	for (i = 0; i < sizeof(data2) / sizeof(*data2); ++i)
		bst_add_recursive(&root2, data2[i]);

	printf("Stabla %s jednaka\n", tree_equal(root1, root2) ? "JESU" : "NISU");

	tree_free(root1);
	tree_free(root2);

	getchar();
	getchar();
	return 0;

}