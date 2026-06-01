#include "route_base.h"

#include <iostream>

route_ns::route_base::route_base(const std::pair<float, float> &start, const std::pair<float, float> &end, const std::vector<node>& v_nodes) : start(start), end(end), v_nodes(v_nodes) {}

void route_ns::route_base_hendler::resize(float w, float h) {this->w=w; this->h=h; plnr.resize(w, h);}

void route_ns::route_base::set_goal_id(int goal_id) {
    this->goal_id = goal_id;
}

std::mutex &route_ns::route_base::get_mutex() const {
    return render_mutex;
}

route_ns::route_base_hendler::route_base_hendler(const space &spc, float w, float h, route_base* routes) : spc(spc), w(w), h(h), start(w/2,h/2), routes(routes), plnr(w,h) {
    end = start;
    set_start(start.first, start.second);
}

void route_ns::route_base_hendler::set_start(float x, float y) {
    start = {x,y};

    routes->clear();
    v_nodes.clear();
    kdt.clear();

    v_nodes.emplace_back(0, x, y);
    kdt.add(v_nodes.front().id, v_nodes.front().x, v_nodes.front().y);
    routes->add(v_nodes.front());
}

void route_ns::route_base_hendler::set_end(float x, float y) {
    set_start(start.first, start.second);
    end = {x,y};
}

void route_ns::route_base_hendler::set_dist_way(float dist_way) {
    this->dist_way= dist_way;
}

std::pair<float, float> route_ns::route_base_hendler::get_start() {
    return start;
}
std::pair<float, float> route_ns::route_base_hendler::get_end() {
    return end;
}

const route_ns::route_base* route_ns::route_base_hendler::get_routes() {
    return routes;
}


void route_ns::route_base_hendler::start_routing(int iters) {
    start_time = std::chrono::steady_clock::now();
    for (int i = 0; i < iters; i++) {
        std::pair<float, float> new_point = plnr.get_random_point();
        if (add(new_point.first, new_point.second))
            break;
        // if (i%100 == 0) std::cout<<"time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - start_time ).count() / 1000 << " points: "<< v_nodes.size()<<std::endl;
    }
    std::cout<<"Spent time: "<< std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - start_time ).count() / 1000 <<" ms."<<std::endl;
}

void route_ns::route_base_hendler::clear() {
    finish_id = 0;
    routes->set_goal_id(0);
    set_start(start.first, start.second);
}

route_ns::route_base_hendler::~route_base_hendler() {
    delete routes;
}
