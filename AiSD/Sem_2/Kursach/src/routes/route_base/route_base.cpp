#include "route_base.h"

#include "planer/planer.h"


route_ns::route_base::route_base(const std::pair<float, float> &start, const std::pair<float, float> &end, const std::vector<node>& v_nodes) : start(start), end(end), v_nodes(v_nodes) {}

void route_ns::route_base::set_goal_id(int goal_id) {
    this->goal_id = goal_id;
}

std::mutex &route_ns::route_base::get_mutex() const {
    return render_mutex;
}

route_ns::route_base_hendler::route_base_hendler(const space &spc, float w, float h, route_base* routes) : spc(spc), w(w), h(h), start(w/2,h/2), routes(routes) {
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

const route_ns::route_base* route_ns::route_base_hendler::get_routes() {
    return routes;
}


void route_ns::route_base_hendler::start_routing(int iters) {
    planer pln(w, h);
    for (int i = 0; i < iters; i++) {
        std::pair<float, float> new_point = pln.get_random_point();
        if (add(new_point.first, new_point.second))
            return;
    }
}

void route_ns::route_base_hendler::clear() {
    set_start(start.first, start.second);
}

route_ns::route_base_hendler::~route_base_hendler() {
    delete routes;
}
