#include "Tree.h"

Node* Tree::findMax(Node*& node) {
    if (!node || !*node) return nullptr;
    Node* cur = node;
    while (cur->rightSub && *cur->rightSub) cur = cur->rightSub;
    return cur;
}

Node* Tree::findMin(Node*& node) {
    if (!node || *node) return nullptr;
    Node* cur = node;
    while (cur->leftSub && *cur->leftSub) cur = cur->leftSub;
    return cur;
}

void Tree::preOrder(Node*& node, std::stringstream& ss) {
    if (node) {
        ss << node->printData() << ' ';
        preOrder(node->leftSub, ss);
        preOrder(node->rightSub, ss);
    }
}

void Tree::inOrder(Node*& node, std::stringstream& ss) {
    if (node) {
        inOrder(node->leftSub, ss);
        ss << node->printData() << ' ';
        inOrder(node->rightSub, ss);
    }
}

void Tree::postOrder(Node*& node, std::stringstream& ss) {
    if (node) {
        postOrder(node->leftSub, ss);
        postOrder(node->rightSub, ss);
        ss << node->printData() << ' ';
    }
}

Node* Tree::rightRotate(Node* node) {
    Node* rootNew = node->leftSub;
    if (root == node) root = rootNew;
    Node* leftNew = rootNew->rightSub;
    if (node->parent) node->parent->makeChild(rootNew, node->edge);
    else rootNew->parent = nullptr;
    rootNew->setRightSub(node);
    node->setLeftSub(leftNew);
    return rootNew;
}

Node* Tree::bigRightRotate(Node* node) {
    leftRotate(node->leftSub);
    return rightRotate(node);
}

Node* Tree::leftRotate(Node* node) {
    Node* rootNew = node->rightSub;
    if (root == node) root = rootNew;
    Node* rightNew = rootNew->leftSub;
    if (node->parent) node->parent->makeChild(rootNew, node->edge);
    else rootNew->parent = nullptr;
    rootNew->setLeftSub(node);
    node->setRightSub(rightNew);
    return rootNew;
}

Node* Tree::bigLeftRotate(Node* node) {
    rightRotate(node->rightSub);
    return leftRotate(node);
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
        balancing(newNode);
        return true;
    }
    Node* cur = root;
    while (cur) {
        if (cur->leftSub && *cur->leftSub && newNode->key < cur->key) cur = cur->leftSub;
        else if (cur->rightSub && *cur->rightSub && newNode->key > cur->key) cur = cur->rightSub;
        else break;
    }
    if (cur->key == newNode->key) {
        delete newNode;
        return false;
    }
    cur->addSub(newNode);

    balancing(newNode);
    return true;
}

bool Tree::insert(const size_t& key) {
    return insert(new Node(key));
}

bool Tree::del(const size_t& key) {
    return del(find(key));
}

bool Tree::del(Node* node) {
    if (!node) return false;
    Node* tempNode = nullptr;
    while (node->leftSub && *node->leftSub) {
        tempNode = findMax(node->leftSub);
        flip(node, tempNode);
        node = tempNode;
    }
    tempNode = node->rightSub;
    delBalancing(node);
    if (node->parent)
        node->parent->makeChild(tempNode, node->edge);
    else if(tempNode) tempNode->clearParent();
    if (root == node) root = tempNode;
    delete node;
    return true;
}

void Tree::flip(Node* node1, Node* node2) {
    Node tempNode(148);
    tempNode.copyData(node1);
    node1->copyData(node2);
    node2->copyData(&tempNode);
}

void Tree::clear() {
    std::queue<Node*> queueOfNodes;
    queueOfNodes.push(root);

    while (!queueOfNodes.empty()) {
        Node* cur = queueOfNodes.front();

        if (cur && *cur) {
            queueOfNodes.push(cur->leftSub);
            queueOfNodes.push(cur->rightSub);
            delete cur;
        }
        queueOfNodes.pop();
    }
    root = nullptr;
}

Node* Tree::findMax() {
    return findMax(root);
}

Node* Tree::findMin() {
    return findMin(root);
}

std::string Tree::preOrder() {
    std::stringstream ss;
    preOrder(root, ss);
    return ss.str();
}

std::string Tree::inOrder() {
    std::stringstream ss;
    inOrder(root, ss);
    return ss.str();
}

std::string Tree::postOrder() {
    std::stringstream ss;
    postOrder(root, ss);
    return ss.str();
}

std::string Tree::levelOrder() {
    std::stringstream ss;

    std::queue<Node*> queueOfNodes;
    queueOfNodes.push(root);

    std::queue<size_t> queueOfLvl;
    queueOfLvl.push(1);

    size_t lvl = 0;
    while (!queueOfNodes.empty()) {
        Node* cur = queueOfNodes.front();
        size_t& nowLvl = queueOfLvl.front();
        if (nowLvl != lvl) {
            ss << '\n' << nowLvl << ": ";
            lvl = nowLvl;
        }
        if (cur) {
            ss << cur->printData() << ' ';
            queueOfNodes.push(cur->leftSub);
            queueOfNodes.push(cur->rightSub);
            queueOfLvl.push(nowLvl + 1);
            queueOfLvl.push(nowLvl + 1);
        }
        queueOfNodes.pop();
        queueOfLvl.pop();
    }
    return ss.str();
}

size_t Tree::getHeight() {
    std::queue<Node*> queueOfNodes;
    queueOfNodes.push(root);

    std::queue<size_t> queueOfLvl;
    queueOfLvl.push(1);

    size_t lvl = 0;
    while (!queueOfNodes.empty()) {
        Node* cur = queueOfNodes.front();
        if (cur) {
            lvl = queueOfLvl.front();
            queueOfNodes.push(cur->leftSub);
            queueOfNodes.push(cur->rightSub);
            queueOfLvl.push(lvl + 1);
            queueOfLvl.push(lvl + 1);
        }
        queueOfNodes.pop();
        queueOfLvl.pop();
    }
    return lvl;
}

Tree::~Tree() {
    clear();
}

