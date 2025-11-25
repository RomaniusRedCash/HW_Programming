#pragma once

class Node {
public:
    Node* leftSub = nullptr;
    Node* rightSub = nullptr;
    Node* parent = nullptr;
    Node** edge = nullptr;
    size_t key;
    Node(const size_t& key) : key(key) {}
    virtual void addSub(Node*& node);
    //void dellSub(Node*& node);
    ~Node();
};

class AVLNode : public Node {
    int blanceFactor = 0;
};

namespace rb {
    enum : char {
        red, black
    };
};

class RBNode : public Node {
    char color = rb::red;
};
