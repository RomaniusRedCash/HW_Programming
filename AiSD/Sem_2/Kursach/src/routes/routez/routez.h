#pragma once

#include "../route_base/route_base.h"

namespace route_ns {
    class routez : public route_base {
    public:
        routez(const std::pair<float, float> &start, const std::pair<float, float> &end, const std::vector<node> &v_nodes);
        void add(const node &n) override;
        void render(SDL_Renderer *renderer) const override;
        void clear() override {}
    };

    class routez_hendler : public route_base_hendler {
        inline static const float radius = 3.0f;
        void update_children_cost(int parent_id);

      public:
        routez_hendler(const space &spc, float w, float h);
        virtual bool add(float x, float y);
    };
}
