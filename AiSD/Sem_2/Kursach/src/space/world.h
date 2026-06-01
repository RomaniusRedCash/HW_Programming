#pragma once

#include "space.h"
#include "../window/window.h"
#include "../routes/route/route.h"
#include "../routes/routez/routez.h"
#include "../routes/fast_route/fast_route.h"
// #include "../route/planer/planer.h"
#include <list>
#include <iostream>
#include <variant>

enum class rrt_type {
    standart, zvezda, fast
};

class world {
public:
    space spc;
    window win;
    route_ns::route_base_hendler* rth;
    // planer plnr;

    bool block_map = false;

    int w,h;

    std::list<shape> l_shapes;
    world(int w, int h) : w(w), h(h), win(w, h), rth(new route_ns::route_hendler(spc, w,h))/*, plnr(w,h)*/ {
        win.add_object(rth->get_routes());
    }
    size_t add_object(const shape& shp) {
        l_shapes.push_back(shp);
        win.add_object(&l_shapes.back());
        spc.add_object(&l_shapes.back());
        return l_shapes.size() - 1;
    }
    void resize(int w, int h) {
        this->w = w;
        this->h = h;
        win.set_size(w,h);
        rth->resize(w,h);
        // plnr.resize(w, h);
    }
    std::pair<int, int> get_size() const {
        return win.get_size();
    }
    void turn(rrt_type type) {
        std::pair<float, float> start = rth->get_start(), end = rth->get_end();
        win.clear();
        rth->clear();
        delete rth;
        switch (type) {
            case rrt_type::standart:
                rth = new route_ns::route_hendler(spc, w,h);
                break;
            case rrt_type::zvezda:
                rth = new route_ns::routez_hendler(spc, w,h);
                break;
            case rrt_type::fast:
                rth = new route_ns::fast_route_hendler(spc, w,h);
                break;
            default:
                break;
        }
        rth->set_start(start.first, start.second);
        rth->set_end(end.first, end.second);
        // clear();
        win.add_object(rth->get_routes());
        for (shape& shp : l_shapes)
            win.add_object(&shp);
    }
    void clear() {
        // win.clear();
        // spc.clear();
        rth->clear();
        for (shape& shp : l_shapes)
            shp = remove_obj();
        win.add_object(rth->get_routes());
    }
    void del(size_t id) {
        if (id >= l_shapes.size()) {
            std::cerr<<"ERROR! Can't remove object."<<std::endl;
        }
        size_t id2 = 0;
        for (std::list<shape>::iterator it = l_shapes.begin(); it != l_shapes.end(); it++) {
            if (id == id2){
                // l_shapes.erase(it);
                // std::visit([](object& concrete_shape){concrete_shape.set_color({0,0,0,0});}, *it);
                // shape shp = remove_obj();
                *it = remove_obj();
                break;
            }
            id2++;
        }
    }
    // void destroy() {
    //     clear();
    //     win.destroy_window();
    //     delete rth;
    // }
    ~world() {
        delete rth;
    }
};
