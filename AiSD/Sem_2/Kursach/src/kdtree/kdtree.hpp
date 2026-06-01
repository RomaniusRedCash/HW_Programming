#pragma once

#include "kdtree.h"
#include "../Header.h"

using namespace kdtree_ns;

template <typename T>
void kdtree_ns::kdtree<T>::clear_recursive(node<T>* current) {
    if (!current) return;
    clear_recursive(current->left);
    clear_recursive(current->right);
    delete current;
}

template <typename T>
void kdtree<T>::add(const T &t, float x, float y) {
    node<T> *n = new node<T>(x,y,t);
    if (!root) {
        root = n;
        return;
    }

    node<T>* n_tmp = root;
    bool is_x = true;

    while (true) {
        bool go_left = is_x ? (n->x < n_tmp->x) : (n->y < n_tmp->y);
        is_x = !is_x;
        if (go_left) {
            if (n_tmp->left) {
                n_tmp = n_tmp->left;
                continue;
            }
            n_tmp->left = n;
            break;
        } else {
            if (n_tmp->right) {
                n_tmp = n_tmp->right;
                continue;
            }
            n_tmp->right = n;
            break;
        }
    }
}

template <typename T>
void kdtree_ns::kdtree<T>::find_in_radius(node<T> *n_current, float target_x, float target_y, float radius, std::vector<T> &result, bool is_x) {
    if (!n_current) return;

    // Считаем квадрат расстояния (так быстрее, без std::sqrt)
    float dx = target_x - n_current->x;
    float dy = target_y - n_current->y;
    float dist_sq = dx * dx + dy * dy;

    // Если узел попал в радиус — добавляем его ID (data) в результат
    if (dist_sq <= radius * radius) {
        result.push_back(n_current->data);
    }

    // Решаем, в какую ветку идти сначала
    bool go_left = is_x ? (target_x < n_current->x) : (target_y < n_current->y);
    node<T> *n_c_1 = go_left ? n_current->left : n_current->right;
    node<T> *n_c_2 = go_left ? n_current->right : n_current->left;

    // Обязательно обыскиваем основную ветку
    find_in_radius(n_c_1, target_x, target_y, radius, result, !is_x);

    // Проверяем, может ли вторая ветка ТЕОРЕТИЧЕСКИ пересекать наш круг поиска
    float axis_dist = is_x ? (target_x - n_current->x) : (target_y - n_current->y);
    if ((axis_dist * axis_dist) <= radius * radius) {
        find_in_radius(n_c_2, target_x, target_y, radius, result, !is_x);
    }
}

template <typename T>
double kdtree_ns::kdtree<T>::get_distance(node<T> *n1, node<T> *n2) {
    return get_distance2(n1->x, n1->y, n2->x, n2->y);
}

template <typename T>
void kdtree_ns::kdtree<T>::down(node<T> *n_current, node<T> *n_target, bool is_x, node<T> *&n_best, double &best_dist) {
    if (!n_current) return;
    double current_dist = get_distance(n_target, n_current);

    if (current_dist < best_dist) {
        best_dist = current_dist;
        n_best = n_current;
    }
    bool go_left = is_x ? (n_target->x < n_current->x) : (n_target->y < n_current->y);
    node<T> *n_c_1 = go_left ? n_current->left : n_current->right;
    node<T> *n_c_2 = go_left ? n_current->right : n_current->left;

    down(n_c_1, n_target, !is_x, n_best, best_dist);

    double axis_dist = is_x ? (n_target->x - n_current->x) : (n_target->y - n_current->y);

    if ((axis_dist * axis_dist) < best_dist) {
        down(n_c_2, n_target, !is_x, n_best, best_dist);
    }
}

template <typename T>
kdtree_ns::node<T>* kdtree_ns::kdtree<T>::operator[](float x, float y) {
    node<T> n_target(x,y);
    node<T>* n_tmp = root;
    double dist = get_distance(n_tmp, &n_target);
    down(n_tmp, &n_target, true, n_tmp, dist);
    return n_tmp;
}

template <typename T>
std::vector<T> kdtree_ns::kdtree<T>::get_nodes_in_radius(float x, float y, float radius) {
    std::vector<T> result;
    find_in_radius(root, x, y, radius, result, true);
    return result;
}

template <typename T>
inline void kdtree_ns::kdtree<T>::clear() {
    clear_recursive(root);
    root = nullptr;
}

template <typename T>
inline kdtree_ns::kdtree<T>::~kdtree() {
    clear();
}
