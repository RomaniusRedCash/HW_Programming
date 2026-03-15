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

    virtual Node* reBalancing(Node* root) { return nullptr; };
    virtual void balancing(Node* node) { return; };
    virtual void delBalancing(Node* node) { return; };

    virtual Node* rightRotate(Node* node);
    virtual Node* bigRightRotate(Node* node);
    virtual Node* leftRotate(Node* node);
    virtual Node* bigLeftRotate(Node* node);

    virtual bool del(Node* node);

    virtual void flip(Node* node1, Node* node2);

public:
    Node* find(const size_t& key);
    virtual bool insert(Node* newNode);
    virtual bool insert(const size_t& key);
    bool del(const size_t& key);
    void clear();

    Node* findMax();
    Node* findMin();

    std::string preOrder();
    std::string inOrder();
    std::string postOrder();
    std::string levelOrder();
    size_t getHeight();

    virtual ~Tree();
};
