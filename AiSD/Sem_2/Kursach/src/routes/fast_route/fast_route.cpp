#include "fast_route.h"

#include "../routez/routez.h"

#include <cmath>
#include <random>
#include <iostream>

route_ns::fast_route_hendler::fast_route_hendler(const space &spc, float w, float h) : route_base_hendler(spc, w, h, new fast_route(start, end, v_nodes)) {
}

std::vector<route_ns::node> route_ns::fast_route_hendler::extract_path(int f_id) {
    std::vector<node> path;
    if (f_id <= 0 || f_id >= v_nodes.size())
        return path;

    int current_id = f_id;
    while (current_id != -1) {
        path.push_back(v_nodes[current_id]);
        current_id = v_nodes[current_id].parant_id;
    }

    std::reverse(path.begin(), path.end());

    return path;
}

std::vector<route_ns::node> route_ns::fast_route_hendler::path_fusion(
    const std::vector<node> &path_A,
    const std::vector<node> &path_B) {
    if (path_A.empty())
        return path_B;
    if (path_B.empty())
        return path_A;

    std::vector<node> fused_path;

    float fusion_radius = dist_way * 1.2f;

    size_t i = 0;
    size_t j = 0;

    fused_path.push_back(path_A[0]);

    while (i < path_A.size() - 1 && j < path_B.size() - 1) {
        bool intersection_found = false;
        size_t next_i = i + 1;
        size_t next_j = j + 1;

        for (size_t ai = i + 1; ai < path_A.size(); ++ai) {
            for (size_t bj = j + 1; bj < path_B.size(); ++bj) {
                float dx = path_A[ai].x - path_B[bj].x;
                float dy = path_A[ai].y - path_B[bj].y;
                float dist = std::sqrt(dx * dx + dy * dy);

                if (dist <= fusion_radius) {
                    next_i = ai;
                    next_j = bj;
                    intersection_found = true;
                    break;
                }
            }
            if (intersection_found)
                break;
        }

        if (intersection_found) {
            float len_A = 0;
            for (size_t k = i; k < next_i; ++k) {
                float dx = path_A[k + 1].x - path_A[k].x;
                float dy = path_A[k + 1].y - path_A[k].y;
                len_A += std::sqrt(dx * dx + dy * dy);
            }

            float len_B = 0;
            for (size_t k = j; k < next_j; ++k) {
                float dx = path_B[k + 1].x - path_B[k].x;
                float dy = path_B[k + 1].y - path_B[k].y;
                len_B += std::sqrt(dx * dx + dy * dy);
            }

            if (len_A <= len_B) {
                for (size_t k = i + 1; k <= next_i; ++k) {
                    // path_A[k].parant_id=fused_path.back().id;
                    fused_path.push_back(path_A[k]);
                }
            } else {
                for (size_t k = j + 1; k <= next_j; ++k) {
                    fused_path.push_back(path_B[k]);
                }
            }

            i = next_i;
            j = next_j;
        } else {
            for (size_t k = i + 1; k < path_A.size(); ++k) {
                fused_path.push_back(path_A[k]);
            }
            break;
        }
    }

    return fused_path;
}
std::vector<route_ns::node> route_ns::fast_route_hendler::path_fine_tuning(const std::vector<node> &intermediate_path) {
    const std::lock_guard<std::mutex> lock(routes->get_mutex());

    if (intermediate_path.size() <= 2)
        return intermediate_path;

    std::vector<node> optimized_path;
    optimized_path.push_back(intermediate_path[0]);
    size_t current = 0;

    while (current < intermediate_path.size() - 1) {
        size_t best_visible_index = current + 1;

        for (size_t look_ahead = current + 2; look_ahead < intermediate_path.size(); ++look_ahead) {
            if (!spc.line_collision(intermediate_path[current].x, intermediate_path[current].y,
                intermediate_path[look_ahead].x, intermediate_path[look_ahead].y)) {
                best_visible_index = look_ahead;
            }
        }
        // v_nodes[intermediate_path[best_visible_index].id].parant_id=optimized_path.back().id;
        optimized_path.push_back(intermediate_path[best_visible_index]);

        current = best_visible_index;
    }

    return optimized_path;
}


bool route_ns::fast_route_hendler::add(float x, float y) {
    const std::lock_guard<std::mutex> lock(routes->get_mutex());
    kdtree_ns::node<int> *kn_check = kdt[x, y];
    if (kn_check) {
        float check_dx = x - v_nodes[kn_check->data].x;
        float check_dy = y - v_nodes[kn_check->data].y;
        float check_dist = std::sqrt(check_dx * check_dx + check_dy * check_dy);

        if (check_dist < r_exclusion) {
            return false;
        }
    }

    int near_id = kn_check->data;
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

    bool collision = spc.line_collision(near_x, near_y, new_x, new_y);

    if (collision) {
        static std::uniform_real_distribution<float> dis_angle(0.0f, 2.0f * M_PI);
        float random_angle = dis_angle(plnr.get_gen());

        new_x = near_x + std::cos(random_angle) * dist_way;
        new_y = near_y + std::sin(random_angle) * dist_way;

        new_x = std::max(0.0f, std::min(new_x, w));
        new_y = std::max(0.0f, std::min(new_y, h));

        if (spc.line_collision(near_x, near_y, new_x, new_y)) {
            return false;
        }

        distance = dist_way;
    }

    float segment_length = std::min(distance, dist_way);
    float new_cost = v_nodes[near_id].cost + segment_length;
    int new_id = v_nodes.size();

    v_nodes.push_back({new_id, new_x, new_y, new_cost, near_id});
    kdt.add(v_nodes.back().id, v_nodes.back().x, v_nodes.back().y);
    float to_goal_x = end.first - new_x;
    float to_goal_y = end.second - new_y;
    float dist_to_goal = std::sqrt(to_goal_x * to_goal_x + to_goal_y * to_goal_y);

    if (dist_to_goal <= dist_way && !spc.line_collision(new_x, new_y, end.first, end.second)) {
        finish_id = v_nodes.size();
        float finish_cost = new_cost + dist_to_goal;

        v_nodes.push_back({finish_id, end.first, end.second, finish_cost, new_id});
        kdt.add(finish_id, end.first, end.second);
        routes->set_goal_id(finish_id);

        std::cout << "Improved-RRT Found Base Path! Cost: " << finish_cost << std::endl;
        return true;
    }

    return false;
}

void route_ns::fast_route_hendler::start_routing(int iters) {
    start_time = std::chrono::steady_clock::now();
    clear();

    bool found_A = false;
    for (int i = 0; i < iters; i++) {
        std::pair<float, float> new_point = plnr.get_random_point();
        if (add(new_point.first, new_point.second)) {
            found_A = true;
            break;
        }
    }
    auto path_A = extract_path(finish_id);
    bool found_B = false;
    clear();
    for (int i = 0; i < iters; ++i) {
        std::pair<float, float> new_point = plnr.get_random_point();
        if (add(new_point.first, new_point.second)) {
            found_B = true;
            break;
        }
    }
    auto path_B = extract_path(finish_id);

    auto fused = path_fusion(path_A, path_B);

    auto final_smooth_path = path_fine_tuning(fused);

    float final_cost = 0;
    for (size_t i = 0; i < final_smooth_path.size() - 1; ++i) {
        float dx = final_smooth_path[i+1].x - final_smooth_path[i].x;
        float dy = final_smooth_path[i+1].y - final_smooth_path[i].y;
        final_cost += std::sqrt(dx * dx + dy * dy);
    }

    static_cast<fast_route*>(routes)->finalv = final_smooth_path;

    std::cout << "Fast-RRT Complete! Optimal Cost: " << final_cost << std::endl;
    std::cout<<"Spent time: "<< std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - start_time ).count() / 1000 <<" ms."<<std::endl;
}
void route_ns::fast_route::render(SDL_Renderer *renderer) const {
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

    for (size_t i = 0; i < finalv.size() - 1; i++){
        line ln(finalv[i].x, finalv[i].y, finalv[i+1].x, finalv[i+1].y);
        ln.set_color(color_of_goal_path);
        ln.render(renderer);
        circle cr(finalv[i+1].x, finalv[i+1].y, radius_of_point);
        cr.set_color(color_of_point);
        cr.render(renderer);
        if (finalv[i+1].x == end.first && finalv[i+1].y ==end.second) break;
    }
}
route_ns::fast_route::fast_route(const std::pair<float, float> &start, const std::pair<float, float> &end, const std::vector<node> &v_nodes)  : routez(start,end,v_nodes) {}
