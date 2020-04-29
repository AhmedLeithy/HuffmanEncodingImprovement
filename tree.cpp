#include "tree.h"

node* tree::combineNodes(node* left, node* right)
{
	node* returned = new node();
	returned->left = left;
	returned->right = right;

	return returned;
}
