#include "RBTree.h"

void RBTree::balancing(Node* node) {
    RBNode* cur = static_cast<RBNode*>(node);
    if (!cur->leftSub)
        cur->setLeftSub(pseudoRBNode);
    if (!cur->rightSub)
        cur->setRightSub(pseudoRBNode);
    if (node == root) {
        cur->setColor(black);
        return;
    }
    if (node->parent == root) return;

    RBNode* parentNode = static_cast<RBNode*>(cur->parent);
    RBNode* grandParentNode;
    RBNode* uncleNode;
    bool isLeft = false;
    while (parentNode->color == red && parentNode->parent) {
        grandParentNode = static_cast<RBNode*>(parentNode->parent);
        if (isLeft = parentNode == grandParentNode->leftSub)
            uncleNode = static_cast<RBNode*>(grandParentNode->rightSub);
        else
            uncleNode = static_cast<RBNode*>(grandParentNode->leftSub);
        if (uncleNode->color == red) {
            uncleNode->setColor(black);
            parentNode->setColor(black);
            grandParentNode->setColor(red);
            cur = grandParentNode;
            if (cur == root) break;
            parentNode = static_cast<RBNode*>(cur->parent);
            continue;
        }
        if (isLeft) {
            if (cur == parentNode->rightSub) {
                leftRotate(parentNode);
                parentNode = static_cast<RBNode*>(cur);
            }
            rightRotate(grandParentNode);
            grandParentNode->setColor(red);
            parentNode->setColor(black);
        }
        else {
            if (cur == parentNode->leftSub) {
                rightRotate(parentNode);
                parentNode = static_cast<RBNode*>(cur);
            }
            leftRotate(grandParentNode);
            grandParentNode->setColor(red);
            parentNode->setColor(black);
        }
        break;
    }
    static_cast<RBNode*>(root)->setColor(black);
}

void RBTree::delBalancing(Node* node) {
    if (!node) return;
    if (node == root) return;
    RBNode* cur = static_cast<RBNode*>(node);
    if (cur->color == red)
        return;
    RBNode* childRBNode = static_cast<RBNode*>(cur->leftSub);
    if (childRBNode->color == red) {
        childRBNode->setColor(black);
        return;
    }/*
    childRBNode = static_cast<RBNode*>(cur->rightSub);
    if (childRBNode->color == red) {
        childRBNode->setColor(black);
        return;
    }*/

    RBNode* parentRBNode = static_cast<RBNode*>(cur->parent);
    bool isLeft = cur == parentRBNode->leftSub;
    RBNode* brotherRBNode;
    if (isLeft)
        brotherRBNode = static_cast<RBNode*>(parentRBNode->rightSub);
    else 
        brotherRBNode = static_cast<RBNode*>(parentRBNode->leftSub);

    if (brotherRBNode->color == red) { // case 1
        if (isLeft) {
            leftRotate(parentRBNode);
            brotherRBNode = static_cast<RBNode*>(parentRBNode->rightSub);
        }
        else {
            brotherRBNode = static_cast<RBNode*>(parentRBNode->leftSub);
            rightRotate(parentRBNode);
        }
        parentRBNode->setColor(red);
        static_cast<RBNode*>(parentRBNode->parent)->setColor(black);
    }

    // барт не красный 100%

    RBNode* leftCusineRBNode = static_cast<RBNode*>(brotherRBNode->leftSub);
    RBNode* rightCusineRBNode = static_cast<RBNode*>(brotherRBNode->rightSub);

    if (leftCusineRBNode->color == black && rightCusineRBNode->color == black) { // case 2
        brotherRBNode->setColor(red);
        if (parentRBNode->color == red) {
            parentRBNode->setColor(black);
            return;
        }
        else return delBalancing(parentRBNode);
    }
    if (isLeft){
        if (leftCusineRBNode->color == red) {
            rightRotate(brotherRBNode);
            brotherRBNode->setColor(red);
            leftCusineRBNode->setColor(black);
            brotherRBNode = static_cast<RBNode*>(parentRBNode->rightSub);
            leftCusineRBNode = static_cast<RBNode*>(brotherRBNode->leftSub);
            rightCusineRBNode = static_cast<RBNode*>(brotherRBNode->rightSub);
        }
        if (rightCusineRBNode->color == red) { // case 4
            leftRotate(parentRBNode);
            brotherRBNode->setColor(parentRBNode->color);
            parentRBNode->setColor(black);
            rightCusineRBNode->setColor(black);
            return;
        }
    }
    else {
        if (rightCusineRBNode->color == red) { // case 3
            leftRotate(brotherRBNode);
            brotherRBNode->setColor(red);
            rightCusineRBNode->setColor(black);
            brotherRBNode = static_cast<RBNode*>(parentRBNode->leftSub);
            leftCusineRBNode = static_cast<RBNode*>(brotherRBNode->leftSub);
            rightCusineRBNode = static_cast<RBNode*>(brotherRBNode->rightSub);
        }

        if (leftCusineRBNode->color == red) { // case 4
            rightRotate(parentRBNode);
            brotherRBNode->setColor(parentRBNode->color);
            parentRBNode->setColor(black);
            leftCusineRBNode->setColor(black);
            return;
        }
    }
}

RBTree::RBTree() : Tree() {
    pseudoRBNode = new PseudoRBNode(148);
}

bool RBTree::insert(Node* newNode) {
    if (dynamic_cast<RBNode*>(newNode))
        return Tree::insert(newNode);
    return false;
}

bool RBTree::insert(const size_t& key) {
    return insert(new RBNode(key));
}

RBTree::~RBTree() {
    Tree::~Tree();
    delete pseudoRBNode;
    root = nullptr;
}
