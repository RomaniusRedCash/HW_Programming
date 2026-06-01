#include "routez.h"

#include "../../object/line/line.h"

#include <cmath>
#include <iostream>

route_ns::routez::routez(const std::pair<float, float> &start, const std::pair<float, float> &end, const std::vector<node> &v_nodes) : route_base(start, end, v_nodes) {};

void route_ns::routez::add(const node &n) {};

void route_ns::routez::render(SDL_Renderer *renderer) const {
    const std::lock_guard<std::mutex> lock(get_mutex());
    for (const node& n : v_nodes) {
        if (n.parant_id < 0) continue;
        line ln(n.x, n.y, v_nodes[n.parant_id].x, v_nodes[n.parant_id].y);
        ln.set_color(color_of_path);
        ln.render(renderer);
        circle cr(n.x, n.y, radius_of_point);
        cr.set_color(color_of_point);
        cr.render(renderer);
    }
    circle cr_start(start.first, start.second, radius_of_point * 2);
    cr_start.set_color(color_of_point);
    cr_start.render(renderer);
    circle cr_end(end.first, end.second, radius_of_point * 2);
    cr_end.set_color(color_of_point);
    cr_end.render(renderer);

    if(goal_id == 0) return;

    std::vector<SDL_FPoint> finish_way(1, {v_nodes[goal_id].x, v_nodes[goal_id].y});
    for (int tid = v_nodes[goal_id].parant_id; tid >= 0; tid = v_nodes[tid].parant_id)
        finish_way.emplace_back(v_nodes[tid].x, v_nodes[tid].y);
    set_color(renderer, color_of_goal_path);
    SDL_RenderLines(renderer, finish_way.data(), finish_way.size());
}

void route_ns::routez_hendler::update_children_cost(int parent_id) {
    for (int child_id : v_nodes[parent_id].children_ids) {
        auto& child = v_nodes[child_id];

        float dx = child.x - v_nodes[parent_id].x;
        float dy = child.y - v_nodes[parent_id].y;
        float dist = std::sqrt(dx * dx + dy * dy);

        child.cost = v_nodes[parent_id].cost + dist;

        update_children_cost(child_id);
    }
}

route_ns::routez_hendler::routez_hendler(const space &spc, float w, float h) : route_ns::route_base_hendler(spc, w,h, new routez(start, end, v_nodes)) {
    // routes = new routez(start, end, v_nodes);
    // set_start(start.first, start.second);
}

bool route_ns::routez_hendler::add(float x, float y) {
    std::lock_guard<std::mutex> lock(routes->get_mutex());
    kdtree_ns::node<int> *kn = kdt[x, y];
    if (!kn)
        return false;

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

    if (spc.line_collision(near_x, near_y, new_x, new_y))
        return false;

    const float SEARCH_RADIUS = dist_way * radius;

    int best_parent_id = near_id;
    float min_cost = v_nodes[near_id].cost + std::min(distance, dist_way);

   std::vector<int> near_neighbors = kdt.get_nodes_in_radius(new_x, new_y, SEARCH_RADIUS);

    for (int neighbor_id : near_neighbors) {
        const auto &neighbor = v_nodes[neighbor_id];

        float n_dx = new_x - neighbor.x;
        float n_dy = new_y - neighbor.y;
        float dist_to_neighbor = std::sqrt(n_dx * n_dx + n_dy * n_dy);

        float potential_cost = neighbor.cost + dist_to_neighbor;

        if (potential_cost < min_cost) {
            if (!spc.line_collision(neighbor.x, neighbor.y, new_x, new_y)) {
                min_cost = potential_cost;
                best_parent_id = neighbor_id;
            }
        }
    }

    int new_id = v_nodes.size();
    v_nodes.emplace_back(new_id, new_x, new_y, min_cost, best_parent_id);
    kdt.add(v_nodes.back().id, v_nodes.back().x, v_nodes.back().y);
    v_nodes[best_parent_id].children_ids.push_back(new_id);

    for (int neighbor_id : near_neighbors) {
        if (neighbor_id == new_id || neighbor_id == best_parent_id)
            continue;

        auto &neighbor = v_nodes[neighbor_id];

        float n_dx = neighbor.x - new_x;
        float n_dy = neighbor.y - new_y;
        float dist_to_new = std::sqrt(n_dx * n_dx + n_dy * n_dy);

        float potential_neighbor_cost = min_cost + dist_to_new;

        if (potential_neighbor_cost < neighbor.cost) {
            if (!spc.line_collision(new_x, new_y, neighbor.x, neighbor.y)) {

                std::vector<int>& old_parent_children = v_nodes[neighbor.parant_id].children_ids;
                old_parent_children.erase(
                    std::remove(old_parent_children.begin(), old_parent_children.end(), neighbor_id),old_parent_children.end()
                );

                neighbor.parant_id = new_id;
                neighbor.cost = potential_neighbor_cost;

                v_nodes[new_id].children_ids.push_back(neighbor_id);

                update_children_cost(neighbor.id);
            }
        }
    }

    float to_goal_x = end.first - new_x;
    float to_goal_y = end.second - new_y;
    float dist_to_goal = std::sqrt(to_goal_x * to_goal_x + to_goal_y * to_goal_y);

    static float best_finish_cost = std::numeric_limits<float>::max();

   if (dist_to_goal <= dist_way && !spc.line_collision(new_x, new_y, end.first, end.second)) {
    float potential_finish_cost = min_cost + dist_to_goal;

    if (potential_finish_cost < best_finish_cost) {
        best_finish_cost = potential_finish_cost;

        if (finish_id == 0) {
            finish_id = v_nodes.size();
            routes->set_goal_id(finish_id);
            v_nodes.push_back({finish_id, end.first, end.second, best_finish_cost, new_id});
            kdt.add(finish_id, end.first, end.second);

            v_nodes[new_id].children_ids.push_back(finish_id);
        } else {
            int old_parent_id = v_nodes[finish_id].parant_id;
            std::vector<int>& old_parent_children = v_nodes[old_parent_id].children_ids;
            old_parent_children.erase(
                std::remove(old_parent_children.begin(), old_parent_children.end(), finish_id),
                old_parent_children.end()
            );

            v_nodes[finish_id].parant_id = new_id;
            v_nodes[finish_id].cost = best_finish_cost;

            v_nodes[new_id].children_ids.push_back(finish_id);
        }

        std::cout << "NEW BEST RRT* Cost: " << best_finish_cost << std::endl;
        std::cout << "Spent time: " << (std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - start_time ).count()) / 1000 << " ms." << std::endl;
    }
}

    return false;
}
