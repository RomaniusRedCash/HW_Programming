#pragma once

#include <vector>

namespace kdtree_ns {
    template <typename T>
    struct node {
        float x, y;
        T data;
        node *left=nullptr, *right=nullptr;
        // node(float x, float y, const T& data) : x(x), y(y), data(data) {}
    };

    template <typename T>
    class kdtree {
        node<T>* root = nullptr;

        double get_distance(node<T> *n1, node<T> *n2);
        void down(node<T> *n_current, node<T> *n_target, bool is_x, node<T> *&n_best, double &best_dist);
        void find_in_radius(node<T> *n_current, float target_x, float target_y, float radius, std::vector<T> &result, bool is_x);
        void clear_recursive(node<T>* current);

      public:
        void add(const T &t, float x, float y);
        node<T> *get(node<T> *n);
        node<T> *operator[](float x, float y);
        std::vector<T> get_nodes_in_radius(float x, float y, float radius);
        void clear();
        ~kdtree();
    };
}

#include "kdtree.hpp"
