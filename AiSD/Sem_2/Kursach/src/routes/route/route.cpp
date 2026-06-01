#include "route.h"

#include <cmath>
#include <iostream>
#include <algorithm>
#include <utility>

route_ns::route::route(const std::pair<float, float> &start, const std::pair<float, float> &end, const std::vector<node>& v_nodes) : route_base(start, end, v_nodes) {}

void route_ns::route::add(const node& n) {
    const std::lock_guard<std::mutex> lock(render_mutex);
    node n_parent = n;
    if (n.parant_id != -1) {
        n_parent = v_nodes[n.parant_id];
    }
    for (std::vector<SDL_FPoint>& v : v_points) {
        if (v.back().x == n_parent.x && v.back().y == n_parent.y) {
            v.emplace_back(n.x, n.y);
            return;
        }
    }
    v_points.push_back(std::vector<SDL_FPoint>(1, {n_parent.x, n_parent.y}));
    if (v_points.size() > 1)
        v_points.back().emplace_back(n.x, n.y);
}

void route_ns::route::render(SDL_Renderer *renderer) const {
    const std::lock_guard<std::mutex> lock(get_mutex());
    circle c_root(start.first, start.second, radius_of_point * 2);
    c_root.set_color(color_of_point);
    c_root.render(renderer);

    // if (start != end) {
        circle c_end(end.first, end.second, radius_of_point * 2);
        c_end.set_color(color_of_point);
        c_end.render(renderer);
    // }

    for (std::vector<SDL_FPoint> v : v_points) {
        set_color(renderer, color_of_path);
        SDL_RenderLines(renderer, v.data(), v.size());
        for (size_t i = 1; i < v.size(); i++) {
            circle c(v[i].x, v[i].y, radius_of_point);
            c.set_color(color_of_point);
            c.render(renderer);
        }
    }
    if(goal_id == 0) return;

    std::vector<SDL_FPoint> finish_way(1, {v_nodes[goal_id].x, v_nodes[goal_id].y});
    for (int tid = v_nodes[goal_id].parant_id; tid >= 0; tid = v_nodes[tid].parant_id)
        finish_way.emplace_back(v_nodes[tid].x, v_nodes[tid].y);
    set_color(renderer, color_of_goal_path);
    SDL_RenderLines(renderer, finish_way.data(), finish_way.size());
}

void route_ns::route::clear() {
    const std::lock_guard<std::mutex> lock(get_mutex());
    v_points.clear();
}

route_ns::route_hendler::route_hendler(const space &spc, float w, float h) : route_base_hendler(spc,w, h, new route(start, end, v_nodes)) {
    // routes = new route(start, end, v_nodes);
    // set_start(start.first, start.second);
}

bool route_ns::route_hendler::add(float x, float y) {
    kdtree_ns::node<int> *kn = kdt[x, y];
    if (!kn) {
        std::cerr << "ERROR! empty kdtree." << std::endl;
        throw;
    }

    int near_id = kn->data;
    float near_x = v_nodes[near_id].x;
    float near_y = v_nodes[near_id].y;

    float dx = x - near_x;
    float dy = y - near_y;

    float distance = std::sqrt(dx * dx + dy * dy);

    float new_x = x;
    float new_y = y;

    if (distance > dist_way) {
        new_x = near_x + (dx / distance) * dist_way;
        new_y = near_y + (dy / distance) * dist_way;
    }

    if (spc.line_collision(near_x, near_y, new_x, new_y)) return false;

    float segment_length = std::min(distance, dist_way);
    float new_cost = v_nodes[near_id].cost + segment_length;
    int new_id = v_nodes.size();

    v_nodes.emplace_back(new_id, new_x, new_y, new_cost, near_id);
    kdt.add(new_id, new_x, new_y);
    routes->add(v_nodes.back());


    float to_goal_x = end.first - new_x;
    float to_goal_y = end.second - new_y;
    float dist_to_goal = std::sqrt(to_goal_x * to_goal_x + to_goal_y * to_goal_y);

    if (dist_to_goal <= dist_way) {
        if (!spc.line_collision(new_x, new_y, end.first, end.second)) {

            finish_id = v_nodes.size();
            float finish_cost = new_cost + dist_to_goal;

            if (end.first != new_x && end.second != new_y) {
                v_nodes.emplace_back(finish_id, end.first, end.second, finish_cost, new_id);
                kdt.add(finish_id, end.first, end.second);
                routes->add(v_nodes.back());
            }
            routes->set_goal_id(finish_id);
            std::cout << "SUCCESS! Goal reached. Finish Node ID: " << finish_id << std::endl;
            return true;
        }
    }
    return false;
}
