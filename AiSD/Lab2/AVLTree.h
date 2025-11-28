#pragma once

#include "Tree.h"

class AVLTree : public Tree {
	Node* balancing(Node* node);
	void rightRotate(Node* node) override;
	void bigRightRotate(Node* node) override;
	void leftRotate(Node* node) override;
	void bigLeftRotate(Node* node) override;
public:
	bool insert(Node* newNode) override;
	bool del(Node* node) override;

};

