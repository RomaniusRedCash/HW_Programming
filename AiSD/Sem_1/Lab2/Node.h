#pragma once

#include <string>
#include <sstream>

class Node {
public:
    Node* leftSub = nullptr;
    Node* rightSub = nullptr;
    Node* parent = nullptr;
    Node** edge = nullptr;
    size_t key;

    Node(const size_t& key) : key(key) {}

    void clearParent();
    void addSub(Node* node);
    void setLeftSub(Node* node);
    void setRightSub(Node* node);
    void makeChild(Node* node, Node** subTree);

    void copyData(Node* node);
    virtual std::string printData();

    virtual operator bool() const;

    virtual ~Node() {}
};

class AVLNode : public Node {
public:
    char balanceFactor = 0;

    std::string printData() override;

    AVLNode(const size_t& key) : Node(key) {}
};

enum node_color : char {
    red, black
};

class RBNode : public Node {
public:
    node_color color = node_color::red;

    RBNode(const size_t& key) : Node(key) {}

    std::string printData() override;

    virtual bool setColor(const node_color& newColor);
};

class PseudoRBNode : public RBNode {
public:
    PseudoRBNode(const size_t& key) : RBNode(key) { color = node_color::black; }

    std::string printData() override;
    operator bool() const override;

    bool setColor(const node_color& newColor) override;
};
