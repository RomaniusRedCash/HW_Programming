#pragma once

typedef struct point point;
struct point {double x, y;};

typedef struct node node;
struct node {
    node *parent;
    node *sub[4];
    char is_rotate;
    int num_sub;
    int level;
    point points[4];
    point c_n;
};
static node *node_new(node *parent);

typedef struct tree tree;
struct tree {
    node *root;
    int typ;
    int level;
};
tree *tree_new();
void tree_clear(tree *tr);
void tree_increase(tree *tr);
