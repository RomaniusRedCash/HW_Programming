#pragma once

#include "Tree.h"

class RBTree : public Tree {
	void balancing(Node* node) override;
	void delBalancing(Node* node) override;

	PseudoRBNode* pseudoRBNode;
public:
	RBTree();
	using Tree::del;
	bool insert(Node* newNode) override;
	bool insert(const size_t& key) override;
	~RBTree();
};

