#pragma once

#include "../route_base/route_base.h"
#include "../routez/routez.h"

namespace route_ns {
    class fast_route : public routez {
    public:
      fast_route(const std::pair<float, float> &start, const std::pair<float, float> &end, const std::vector<node> &v_nodes);
      void render(SDL_Renderer *renderer) const override;
      std::vector<node> finalv;
    };

    class fast_route_hendler : public route_base_hendler {
        float r_exclusion = dist_way * 0.8f;

        std::vector<node> extract_path(int f_id);
        std::vector<node> path_fusion(const std::vector<node> &path_A, const std::vector<node> &path_B);
        std::vector<node> path_fine_tuning(const std::vector<node> &intermediate_path);

        std::vector<node> global_best_path;

      public:
        fast_route_hendler(const space &spc, float w, float h);
        bool add(float x, float y) override;
        void start_routing(int iters) override;
    };
}
