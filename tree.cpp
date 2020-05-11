#include "tree.h"

node tree::combineNodes(node& left, node& right)
{
	node *returned = new node();
	node* newLeft = new node(left);
	node* newRight = new node(right);

	returned->left = newLeft;
	returned->right = newRight;

	return *returned;
}

void tree::deleteNode(node* ptr) {
	if(ptr){
		if(ptr->left)
			deleteNode(ptr->left);
		if(ptr->right)
			deleteNode(ptr->right);
		delete ptr;
	}
}

tree::~tree() {
	deleteNode(root);
}