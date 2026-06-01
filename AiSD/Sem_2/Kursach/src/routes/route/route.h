#pragma once

// #include "../kdtree/kdtree.h"
// #include "../../space/space.h"
#include "../route_base/route_base.h"
#include <vector>
// #include <unordered_map>
#include <SDL3/SDL.h>

namespace route_ns {
    class route_hendler;

    class route : public route_base {
        friend route_hendler;

        std::vector<std::vector<SDL_FPoint>> v_points;
    public:
        route(const std::pair<float, float> &start, const std::pair<float, float> &end, const std::vector<node>& v_nodes);
        void add(const node &n) override;
        void render(SDL_Renderer *renderer) const override;
        void clear() override;
    };

    class route_hendler : public route_base_hendler {
    protected:
    public:
        route_hendler(const space &spc, float w, float h);
        bool add(float x, float y) override;
    };
}
