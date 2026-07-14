#include "tree.h"

#include <stdlib.h>

#define FOR4 for (char i = 0; i < 4; i++)
#define set_cord(point, X, Y) \
    point.x = X;              \
    point.y = Y;

static void rotate(point points[4]) {
    point new_points[4];
    FOR4 {
        new_points[i].x = points[i].y;
        new_points[i].y = points[i].x;
    }
    points[0] = new_points[0];
    points[1] = new_points[3];
    points[2] = new_points[2];
    points[3] = new_points[1];
}

static void points_translate(point points[4], double dx, double dy) {
    FOR4 {
        points[i].x+=dx;
        points[i].y+=dy;
    }
}

static void node_init(node *n) {
    n->c_n = (point){300, 350.0 / 2};
    n->num_sub = 0;
    n->is_rotate = 0;
    if (n->parent == NULL) return;
    point parents_points[4];
    node *parent = n->parent;
    point *points = n->points;
    n->level = parent->level + 1;
    FOR4 {set_cord(parents_points[i], parent->points[i].x - parent->points[0].x, parent->points[i].y - parent->points[0].y);}
    if (parent->is_rotate) rotate(parents_points);
    FOR4 {set_cord(points[i], parents_points[i].x * 0.5, parents_points[i].y * 0.5);}
    double width = parents_points[2].x;
    double height = parents_points[2].y;
    switch (n->parent->num_sub) {
        case 0:
            n->is_rotate = !parent->is_rotate;
            rotate(points);
            points_translate(points, 0, 0);
            break;
        case 1:
            n->is_rotate = parent->is_rotate;
            points_translate(points, width / 4, 0);
            break;
        case 2:
            n->is_rotate = parent->is_rotate;
            points_translate(points, width / 4, height / 2);
            break;
        case 3:
            n->is_rotate = !parent->is_rotate;
            rotate(points);
            points_translate(points, 3 * width / 4, 0);
            break;
        default:
            break;

    }
    if (parent->is_rotate) rotate(points);
    points_translate(points, parent->points[0].x, parent->points[0].y);
}

static node *node_new(node *parent) {
    node *n = malloc(sizeof(node));
    n->level = 1;
    n->parent = parent;
    node_init(n);
    return n;
}

static void add_sub(node *parent) {
    if (parent->num_sub > 4)
        exit(-1);
    node *sub = node_new(parent);
    parent->sub[parent->num_sub] = sub;
    parent->num_sub++;
}

static void tree_init(tree *tr) {
    node *n = tr->root = node_new(NULL);
    tr->level = 1;
    set_cord(n->points[0], 50, 50);
    set_cord(n->points[1], 550, 50);
    set_cord(n->points[2], 550, 300);
    set_cord(n->points[3], 50, 300);
}

tree *tree_new() {
    tree *tr = malloc(sizeof(tree));
    tree_init(tr);
    return tr;
}

static void tree_increase_recursive(node *n) {
    if (n->num_sub == 0) FOR4
        add_sub(n);
    else for (int i = 0; i < n->num_sub; i++)
        tree_increase_recursive(n->sub[i]);
}

void tree_increase(tree *tr) {
    tr->level++;
    tree_increase_recursive(tr->root);
}

static void tree_clear_recursive(node *n) {
    for (int i = 0; i < n->num_sub; i++)
        tree_clear_recursive(n->sub[i]);
    free(n);
}

void tree_clear(tree *tr) {
    tree_clear_recursive(tr->root);
    free(tr);
}

#undef set_cord
#undef FOR4
