#include "AVLTree.h"

Node* AVLTree::reBalancing(Node* node) {
    AVLNode* avlRoot = static_cast<AVLNode*>(node);
    if (std::abs(avlRoot->balanceFactor) <= 1) return avlRoot;
    if (avlRoot->balanceFactor > 1) {
        if (static_cast<AVLNode*>(avlRoot->leftSub)->balanceFactor >= 0)
            return rightRotate(avlRoot);
        else
            return bigRightRotate(avlRoot);
    }
    else {
        if (static_cast<AVLNode*>(avlRoot->rightSub)->balanceFactor <= 0)
            return leftRotate(avlRoot);
        else
            return bigLeftRotate(avlRoot);
    }
}

void AVLTree::balancing(Node* node) {
    if (!node) return ;
    if (node == root) return ;
    AVLNode* cur = static_cast<AVLNode*>(node);
    do {
        AVLNode* tempNode = static_cast<AVLNode*>(cur->parent);
        if (cur == tempNode->leftSub)
            tempNode->balanceFactor++;
        else
            tempNode->balanceFactor--;
        cur = tempNode;
    } while (cur->parent && cur->balanceFactor != 0 && std::abs(cur->balanceFactor) <= 1);
    reBalancing(cur);
}

void AVLTree::delBalancing(Node* node) {
    if (!node) return;
    if (!node->parent) return;
    AVLNode* cur = static_cast<AVLNode*>(node);
    do {
        AVLNode* tempNode = static_cast<AVLNode*>(cur->parent);

        if (cur == tempNode->leftSub) 
            tempNode->balanceFactor--;
        else
            tempNode->balanceFactor++;
        cur = tempNode;
        if (std::abs(cur->balanceFactor) > 1)
            cur = static_cast<AVLNode*>(reBalancing(cur));
    } while (cur->parent && std::abs(cur->balanceFactor) != 1);
}

Node* AVLTree::rightRotate(Node* node) {
    AVLNode* A = static_cast<AVLNode*>(node);
    AVLNode* B = static_cast<AVLNode*>(A->leftSub);
    if (A->balanceFactor == 2 && B->balanceFactor == 0) {
        A->balanceFactor = 1;
        B->balanceFactor = -1;
    }
    else {
        A->balanceFactor = 0;
        B->balanceFactor = 0;
    }
    return Tree::rightRotate(node);
}

Node* AVLTree::bigRightRotate(Node* node) {
    AVLNode* A = static_cast<AVLNode*>(node);
    AVLNode* B = static_cast<AVLNode*>(A->leftSub);
    AVLNode* C = static_cast<AVLNode*>(B->rightSub);
    char oldbalanceC = C->balanceFactor;
    Tree::bigRightRotate(node);
    A->balanceFactor = 0;
    B->balanceFactor = 0;
    C->balanceFactor = 0;
    if (oldbalanceC > 0)
        A->balanceFactor = -1;
    else if (oldbalanceC < 0)
        B->balanceFactor = 1;
    return C;
}

Node* AVLTree::leftRotate(Node* node) {
    AVLNode* A = static_cast<AVLNode*>(node);
    AVLNode* B = static_cast<AVLNode*>(A->rightSub);
    if (A->balanceFactor == -2 && B->balanceFactor == 0) {
        A->balanceFactor = -1;
        B->balanceFactor = 1;
    }
    else {
        A->balanceFactor = 0;
        B->balanceFactor = 0;
    }
    return Tree::leftRotate(node);
}

Node* AVLTree::bigLeftRotate(Node* node) {
    AVLNode* A = static_cast<AVLNode*>(node);
    AVLNode* B = static_cast<AVLNode*>(A->rightSub);
    AVLNode* C = static_cast<AVLNode*>(B->leftSub);
    char oldbalanceC = C->balanceFactor;
    Tree::bigLeftRotate(node);
    A->balanceFactor = 0;
    B->balanceFactor = 0;
    C->balanceFactor = 0;
    if (oldbalanceC > 0) 
        B->balanceFactor = -1;
    if (oldbalanceC < 0) 
        A->balanceFactor = 1;
    return C;
}

bool AVLTree::insert(Node* newNode) {
    if (dynamic_cast<AVLNode*>(newNode))
        return Tree::insert(newNode);
    return false;
}
bool AVLTree::insert(const size_t& key) {
    return insert(new AVLNode(key));
}