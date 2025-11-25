#include <iostream>

class Node {
public:
    Node* leftSub = nullptr;
    Node* rightSub = nullptr;
    Node* parent = nullptr;
    size_t key;
    Node(const size_t& key) : key(key) {}
    virtual void addSub(const Node*& node){
        node->parent = this;
        if (node->key < this->key) return leftSub = node;
        if (node->key > this->key) return rightSub = node;
        delete node;
    }
    void dellSub(const Node*& node){
        if (node->key < this->key){
            // delete node->leftSub;
            node->leftSub = nullptr;
        }
        else if (node->key > this->key) {
            // delete node->rightSub;
            node->rightSub = nullptr;
        }
    }
    ~Node{
        if (parent)
            if (parent->leftSub == this) parent->leftSub = nullptr;
            else parent->rightSub = nullptr;
    }
}

class AVLNode : public Node{
    int blance = 0;
}

namespace rb : char {
    enum {
        red, black
    }
}

class RBNode : public Node{
    char color = rb::red;
}


class Tree{
    Node* root = nullptr;
public:
    Node& find(const size_t& key){
        Node* cur = root;
        while(cur){
            if (key < cur->key) cur = cur->leftSub;
            else if (key > cur->key) cur = cur->rightSub;
            else return Node;
        }
        return nullptr;
    }
    virtual void insert(const size_t& key){
        Node* cur = root;
        while(cur){
            if (key < cur->key && cur->leftSub) cur = cur->leftSub;
            else if (key > cur->key && cur->rightSub) cur = cur->rightSub;
            else break;
        }
        if (cur->key == key) return;
        cur->addSub(new Node(key));
    }
    virtual void del(const size_t& key){
        Node*& tempNode find(key);
        if (!tempNode->leftSub)
            if(tempNode->rightSub)
                tempNode->parent->addSub(tempNode->rightSub);
            else tempNode->dellSub();
        else if (!tempNode->rightSub)
            tempNode->parent->addSub(tempNode->leftSub);

    }
}


int main(){



    return 0;
}
