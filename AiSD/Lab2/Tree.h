#pragma once

#include <iostream>
#include <sstream>
#include <queue>

#include "Node.h"

class Tree {
protected:
    Node* root = nullptr;

    Node* findMax(Node*& node);
    Node* findMin(Node*& node);
    void preOrder(Node*& node, std::stringstream& ss);
    void inOrder(Node*& node, std::stringstream& ss);
    void postOrder(Node*& node, std::stringstream& ss);
    //void levelOrder(Node*& node, std::stringstream& ss);
public:
    Node* find(const size_t& key);
    virtual void insert(const size_t& key);
    virtual void del(const size_t& key);
    Node* findMax();
    Node* findMin();
    std::stringstream preOrder();
    std::stringstream inOrder();
    std::stringstream postOrder();
    std::stringstream levelOrder();
    virtual ~Tree();
};