#pragma once

#include "Tree.h"

class AVLTree : public Tree {
	Node* reBalancing(Node* root) override;
	void balancing(Node* node) override;
	void delBalancing(Node* node) override;

	Node* rightRotate(Node* node) override;
	Node* bigRightRotate(Node* node) override;
	Node* leftRotate(Node* node) override;
	Node* bigLeftRotate(Node* node) override;
public:
	using Tree::del;
	bool insert(Node* newNode) override;
	bool insert(const size_t& key) override;
};

