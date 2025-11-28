#include "AVLTree.h"

Node* AVLTree::balancing(Node* node) {
    if (!node) return node;
    AVLNode* cur = static_cast<AVLNode*>(node);
    while(cur->parent && std::abs(cur->balanceFactor) <= 1){
         AVLNode* tempNode = static_cast<AVLNode*>(cur->parent);
        if (*cur->edge == cur->parent->leftSub)
            tempNode->balanceFactor++;
        else
            tempNode->balanceFactor--;
        cur = tempNode;
    }
    return cur;
}

void AVLTree::rightRotate(Node* node) {
    static_cast<AVLNode*>(node)->balanceFactor = 0;
    static_cast<AVLNode*>(node->leftSub)->balanceFactor = 0;
    Tree::rightRotate(node);
}

void AVLTree::bigRightRotate(Node* node) {
    
}

void AVLTree::leftRotate(Node* node) {
    static_cast<AVLNode*>(node)->balanceFactor = 0;
    static_cast<AVLNode*>(node->rightSub)->balanceFactor = 0;
    Tree::leftRotate(node);
}

void AVLTree::bigLeftRotate(Node* node) {

}

bool AVLTree::insert(Node* newNode) {
    //Node* tempNode = new AVLNode(key);
    if(!Tree::insert(newNode)) return false;
    Node* cur = balancing(newNode);
    if (!cur) return true;
    AVLNode* avlRoot = static_cast<AVLNode*>(cur);
    if (std::abs(avlRoot->balanceFactor) < 1) return true;
    if (avlRoot->balanceFactor > 1) {
        AVLNode* avlLeft = static_cast<AVLNode*>(avlRoot->leftSub);
        if (avlLeft->balanceFactor >= 0)
            rightRotate(avlRoot);
        else if (avlLeft->balanceFactor < 0)
            bigRightRotate(avlRoot);
    }
    else if (avlRoot->balanceFactor < -1) {
        AVLNode* avlRight = static_cast<AVLNode*>(avlRoot->rightSub);
        if (avlRight->balanceFactor >= 0)
            leftRotate(avlRoot);
        else if (avlRight->balanceFactor < 0)
            bigLeftRotate(avlRoot);
    }
    return true;
    //balancing(avlRoot);
}
