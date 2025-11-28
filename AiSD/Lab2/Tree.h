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
    virtual void rightRotate(Node* node);
    virtual void bigRightRotate(Node* node);
    virtual void leftRotate(Node* node);
    virtual void bigLeftRotate(Node* node);
public:
    Node* find(const size_t& key);
    virtual bool insert(Node* newNode);
    bool del(const size_t& key);
    virtual bool del(Node* node);
    Node* findMax();
    Node* findMin();
    std::stringstream preOrder();
    std::stringstream inOrder();
    std::stringstream postOrder();
    std::stringstream levelOrder();
    virtual ~Tree();
};
