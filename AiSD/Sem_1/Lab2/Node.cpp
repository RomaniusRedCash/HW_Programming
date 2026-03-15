#include "Node.h"

void Node::clearParent() {
    edge = nullptr;
    parent = nullptr;
}

void Node::addSub(Node* node) {
    if (!node) return;
    node->parent = this;
    if (node->key < this->key)
        setLeftSub(node);
    else if (node->key > this->key)
        setRightSub(node);
    else delete node;
}

void Node::setLeftSub(Node* node) {
    makeChild(node, &leftSub);
}

void Node::setRightSub(Node* node) {
    makeChild(node, &rightSub);
}

void Node::makeChild(Node* node, Node** subTree) {
    *subTree = node;
    if (!node) return;
    node->edge = subTree;
    node->parent = this;
}

void Node::copyData(Node* node) {
    key = node->key;
}

std::string Node::printData() {
    return std::to_string(key);
}

Node::operator bool() const {
    return this;
}

std::string AVLNode::printData() {
    std::stringstream ss;
    ss << '(' << int(balanceFactor) << ':' << key << ')';
    return ss.str();
}

std::string RBNode::printData() {
    std::stringstream ss;
    ss << '(';
    switch (color) {
    case red:
        ss << "red: ";
        break;
    case black:
        ss << "black: ";
        break;
    default:
        break;
    }
    ss << key << ')';
    return ss.str();
}

bool RBNode::setColor(const node_color& newColor) {
    color = newColor; return true;
}

std::string PseudoRBNode::printData() {
    return "*";
}

PseudoRBNode::operator bool() const {
    return false;
}

bool PseudoRBNode::setColor(const node_color& newColor) {
    if (newColor != color) return false;
    else return true;
}