#pragma once

#include "Tree.h"

class AVLTree : public Tree {
	void balancing(Node*& node);
public:
	void insert(const size_t& key) override;
	virtual void del(const size_t& key) override;

};

