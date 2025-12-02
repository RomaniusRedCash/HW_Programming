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
    static_cast<AVLNode*>(node)->balanceFactor = 0;
    static_cast<AVLNode*>(node->leftSub)->balanceFactor = 0;
    return Tree::rightRotate(node);
}

Node* AVLTree::bigRightRotate(Node* node) {
    char oldbalanceC = static_cast<AVLNode*>(node->leftSub->rightSub)->balanceFactor;
    AVLNode* B = static_cast<AVLNode*>(node->leftSub);
    Node* C = Tree::bigRightRotate(node);
    if (oldbalanceC > 0) 
        static_cast<AVLNode*>(node)->balanceFactor = -1;
    if (oldbalanceC < 0) 
        B->balanceFactor = 1;
    return C;
}

Node* AVLTree::leftRotate(Node* node) {
    static_cast<AVLNode*>(node)->balanceFactor = 0;
    static_cast<AVLNode*>(node->rightSub)->balanceFactor = 0;
    return Tree::leftRotate(node);
}

Node* AVLTree::bigLeftRotate(Node* node) {
    int8_t oldbalanceC = static_cast<AVLNode*>(node->rightSub->leftSub)->balanceFactor;
    AVLNode* B = static_cast<AVLNode*>(node->rightSub);
    Node* C = Tree::bigLeftRotate(node);
    if (oldbalanceC < 0) 
        static_cast<AVLNode*>(node)->balanceFactor = 1;
    if (oldbalanceC > 0) 
        B->balanceFactor = -1;
    
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