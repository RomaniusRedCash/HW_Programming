#include "Node.h"

void Node::addSub(Node*& node) {
    if (!node) return;
    node->parent = this;
    if (node->key < this->key) {
        node->edge = &leftSub;
        leftSub = node;
    }
    else if (node->key > this->key) {
        node->edge = &rightSub;
        rightSub = node;
    }
    else delete node;
}

//void Node::dellSub(Node*& node) {
//    if (node->key < this->key) {
//        // delete node->leftSub;
//        node->leftSub = nullptr;
//    }
//    else if (node->key > this->key) {
//        // delete node->rightSub;
//        node->rightSub = nullptr;
//    }
//}

Node::~Node() {
    //if (edge)
    //    *edge = nullptr;
}