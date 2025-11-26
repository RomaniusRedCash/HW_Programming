#include "AVLTree.h"

Node* AVLTree::balancing(Node*& node) {
    if (!node) return node;
    AVLNode* cur = static_cast<AVLNode*>(node);
    while(cur->parent && std::abs(cur->blanceFactor) <= 1){
        AVLNode* tempNode;
        tempNode = static_cast<AVLNode*>(cur->parent);
        if (*cur->edge == cur->parent->leftSub)
            tempNode->balanceFactor--;
        else
            tempNode->balanceFactor++;
        cur = tempNode;
    }
    return cur;
}

void AVLTree::rightRotate(Node*& node){
    Node* rootNew = node->leftSub;
    Node* leftNew = rootNew->rightSub;
    lef
}

void AVLTree::bigRightRotate(Node*& node){

}

void AVLTree::leftRotate(Node*& node){

}

void AVLTree::bigLeftRotate(Node*& node){

}

void AVLTree::insert(const size_t& key) override{
    Node* cur = root;
    if (!cur) return;
    while (cur) {
        if (key < cur->key && cur->leftSub) cur = cur->leftSub;
        else if (key > cur->key && cur->rightSub) cur = cur->rightSub;
        else break;
    }
    if (cur->key == key) return;
    Node* tempNode = new Node(key)
    cur->addSub(tempNode);
    cur = balancing(tempNode);
    if (!cur) return;
    AVLNode* avlRoot = static_cast<AVLNode*>(cur);
    if (std::abs(avlRoot->balanceFactor) < 1) return;
    if (avlRoot->balanceFactor > 1){
        AVLNode* avlRight = static_cast<AVLNode*>(avlRoot->rightSub);
        if (avlRight->balanceFactor >= 0)
            rightRotate(avlRoot);
        else if (avlRight->balanceFactor < 0)
            bigRigtRotate(avlRoot);
    }
    else if (avlRoot->balanceFactor < -1){
        AVLNode* avlRight = static_cast<AVLNode*>(avlRoot->rightSub);
        if (avlRight->balanceFactor >= 0)
            leftRotate(avlRoot);
        else if (avlRight->balanceFactor < 0)
            bigLeftRotate(avlRoot);
    }
}
