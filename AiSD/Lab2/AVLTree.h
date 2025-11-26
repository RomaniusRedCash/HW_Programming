#pragma once

#include "Tree.h"

class AVLTree : public Tree {
	Node* balancing(Node*& node);
	void rightRotate(Node*& node);
	void bigRightRotate(Node*& node);
	void leftRotate(Node*& node);
	void bigLeftRotate(Node*& node);
public:
	void insert(const size_t& key) override;
	virtual void del(const size_t& key) override;

};

