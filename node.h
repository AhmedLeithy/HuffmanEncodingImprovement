#pragma once
#include <string>
using namespace std;
class node
{
public:
	string symbol;
	int frequency;
	node* right;
	node* left;

	bool operator<(const node& right) const {
		return frequency < right.frequency;
	}

	bool operator>(const node& right) const {
		return frequency > right.frequency;
	}

	node(string s, int f) {
		symbol = s;
		frequency = f;
	}

	node(const node& n) {
		symbol = n.symbol;
		frequency = n.frequency;
		right = n.right;
		left = n.left;
	}

	node() {}
};
