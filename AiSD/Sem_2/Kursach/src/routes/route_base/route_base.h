#pragma once

#include <mutex>
#include <chrono>
#include <ctime>

#include "../../kdtree/kdtree.h"
#include "../../space/space.h"
#include "planer/planer.h"

namespace route_ns {
    struct node {
        int id = 0;
        float x = 0, y = 0;
        float cost = 0;
        int parant_id = -1;
        std::vector<int> children_ids;
    };

    class route_base_hendler;

    class route_base : public object {
    protected:
        friend route_base_hendler;
        mutable std::mutex render_mutex;

        int goal_id = 0;
        const std::vector<node>& v_nodes;
        const std::pair<float, float>& start;
        const std::pair<float, float>& end;

        inline static const float radius_of_point = 1;
        inline static const std::array<uint8_t, 4>
            color_of_point = {0xff, 0x00, 0x00, 255}, // #ff0000
            color_of_path = {0x00, 0x40, 0xff, 255}, // #0040ff
            color_of_goal_path = {0x00, 0xff, 0x00, 255}// #00ff00
        ;
    public:
        route_base(const std::pair<float, float> &start, const std::pair<float, float> &end, const std::vector<node>& v_nodes);
        virtual void add(const node &n) = 0;
        void set_goal_id(int goal_id);
        std::mutex &get_mutex() const;
        virtual void render(SDL_Renderer *renderer) const override = 0;
        virtual void clear() = 0;
        virtual ~route_base() = default;
    };

    class route_base_hendler {
    protected:
        kdtree_ns::kdtree<int> kdt;
        const space &spc;
        std::vector<node> v_nodes = {};
        float dist_way = 10;
        std::pair<float, float> start = {0,0}, end = {0,0};
        int finish_id = 0;
        float w,h;
        route_base* routes = nullptr;
        planer plnr;
        std::chrono::time_point<std::chrono::steady_clock> start_time;
    public:
        route_base_hendler(const space &spc, float w, float h, route_base* routes);
        virtual bool add(float x, float y) = 0;
        void resize(float w, float h);
        virtual void set_start(float x, float y);
        virtual void set_end(float x, float y);
        void set_dist_way(float dist_way);
        std::pair<float, float> get_start();
        std::pair<float, float> get_end();
        const route_base* get_routes();
        virtual void start_routing(int iters);
        void clear();
        virtual ~route_base_hendler();
    };
}
