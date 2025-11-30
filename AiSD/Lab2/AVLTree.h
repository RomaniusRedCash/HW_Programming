#pragma once

#include "Tree.h"

class AVLTree : public Tree {
	void reBalancing(Node* root) override;
	void balancing(Node* node) override;
	void delBalancing(Node* node) override;

	void rightRotate(Node* node) override;
	void bigRightRotate(Node* node) override;
	void leftRotate(Node* node) override;
	void bigLeftRotate(Node* node) override;
public:
	using Tree::del;
	bool insert(Node* newNode) override;
	bool insert(const size_t& key) override;
};

