#pragma once
#include "node.h"
class tree
{
	void deleteNode(node* ptr);
public:
	node* root = nullptr;
	node combineNodes(node& left, node& right);
	~tree(){}
};

