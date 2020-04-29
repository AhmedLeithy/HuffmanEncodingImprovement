#include "tree.h"

node* tree::combineNode(node* left, node* right)
{
	node* returned = new node();
	returned->left = left;
	returned->right = right;

	return nullptr;
}
