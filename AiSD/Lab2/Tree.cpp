#include "Tree.h"

Node* Tree::findMax(Node*& node) {
    if (!node) return nullptr;
    Node* cur = node;
    while (cur->rightSub) cur = cur->rightSub;
    return cur;
}

Node* Tree::findMin(Node*& node) {
    if (!node) return nullptr;
    Node* cur = node;
    while (cur->leftSub) cur = cur->leftSub;
    return cur;
}

void Tree::preOrder(Node*& node, std::stringstream& ss) {
    if (node) {
        ss << node->key;
        preOrder(node->leftSub, ss);
        preOrder(node->rightSub, ss);
    }
}

void Tree::inOrder(Node*& node, std::stringstream& ss) {
    if (node) {
        inOrder(node->leftSub, ss);
        ss << node->key;
        inOrder(node->rightSub, ss);
    }
}

void Tree::postOrder(Node*& node, std::stringstream& ss) {
    if (node) {
        postOrder(node->leftSub, ss);
        postOrder(node->rightSub, ss);
        ss << node->key;
    }
}

void Tree::rightRotate(Node* node) {
    Node* rootNew = node->leftSub;
    Node* leftNew = rootNew->rightSub;
    if (node->parent) node->parent->addSub(rootNew);
    rootNew->addSub(node);
    node->addSub(leftNew);
}

void Tree::bigRightRotate(Node* node) {
    leftRotate(node->leftSub);
    rightRotate(node);
}

void Tree::leftRotate(Node* node) {
    Node* rootNew = node->rightSub;
    Node* rightNew = rootNew->leftSub;
    if (node->parent) node->parent->addSub(rootNew);
    //rootNew->parent = node->parent;
    //rootNew->edge = node->edge;
    rootNew->addSub(node);
    node->addSub(rightNew);
}

void Tree::bigLeftRotate(Node* node) {
    rightRotate(node->leftSub);
    leftRotate(node);
}

Node* Tree::find(const size_t& key) {
    Node* cur = root;
    while (cur) {
        if (key < cur->key) cur = cur->leftSub;
        else if (key > cur->key) cur = cur->rightSub;
        else return cur;
    }
    return nullptr;
}

bool Tree::insert(Node* newNode) {
    if (!root) {
        root = newNode;
        return true;
    }
    Node* cur = root;
    while (cur) {
        if (newNode->key < cur->key && cur->leftSub) cur = cur->leftSub;
        else if (newNode->key > cur->key && cur->rightSub) cur = cur->rightSub;
        else break;
    }
    if (cur->key == newNode->key) {
        delete newNode;
        return false;
    }
    cur->addSub(newNode);
    return true;
}

void Tree::del(const size_t& key) {
    Node* deletingNode = find(key);
    if (!deletingNode) return;
    *deletingNode->edge = nullptr;
    if (!deletingNode->leftSub)
        *deletingNode->edge = deletingNode->rightSub;
    else if (!deletingNode->rightSub)
        *deletingNode->edge = deletingNode->leftSub;
    else {
        Node* tempNode = findMax(deletingNode->rightSub);
        *deletingNode->edge = tempNode;
    }
    delete deletingNode;
}

Node* Tree::findMax() {
    return findMax(root);
}

Node* Tree::findMin() {
    return findMin(root);
}

std::stringstream Tree::preOrder() {
    std::stringstream ss;
    preOrder(root, ss);
    return ss;
}

std::stringstream Tree::inOrder() {
    std::stringstream ss;
    inOrder(root, ss);
    return ss;
}

std::stringstream Tree::postOrder() {
    std::stringstream ss;
    postOrder(root, ss);
    return ss;
}

std::stringstream Tree::levelOrder() {
    std::stringstream ss;
    std::queue<Node*> queueOfNodes;
    queueOfNodes.push(root);

    while (!queueOfNodes.empty()) {
        Node* cur = queueOfNodes.front();
        if (cur) {
            ss << cur->key;
            queueOfNodes.push(cur->leftSub);
            queueOfNodes.push(cur->rightSub);
        }
        queueOfNodes.pop();
    }
    return ss;
}

Tree::~Tree() {
    std::queue<Node*> queueOfNodes;
    queueOfNodes.push(root);

    while (!queueOfNodes.empty()) {
        Node* cur = queueOfNodes.front();
        if (cur) {
            queueOfNodes.push(cur->leftSub);
            queueOfNodes.push(cur->rightSub);
        }
        queueOfNodes.pop();
        delete cur;
    }
}

