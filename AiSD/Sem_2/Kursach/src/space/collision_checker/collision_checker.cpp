#include "collision_checker.h"

#include <cmath>

const float delta = 5e-6;

bool collision_checker::operator()(const rectangle &rec, const line &ln) const {
    box_collision bc_rec = rec.get_box_collision();
    float rx1 = bc_rec.x;
    float ry1 = bc_rec.y;
    float rx2 = bc_rec.x + bc_rec.w;
    float ry2 = bc_rec.y + bc_rec.h;

    float lx1 = ln.x1, ly1 = ln.y1;
    float lx2 = ln.x2, ly2 = ln.y2;

    if (std::max(lx1, lx2) < rx1 || std::min(lx1, lx2) > rx2 ||
        std::max(ly1, ly2) < ry1 || std::min(ly1, ly2) > ry2
    ) return false;

    if (lx1 >= rx1 && lx1 <= rx2 && ly1 >= ry1 && ly1 <= ry2 ||
        lx2 >= rx1 && lx2 <= rx2 && ly2 >= ry1 && ly2 <= ry2
    ) return true;

    if (std::abs(lx2 - lx1) < delta) return true;

    float k = (ly2 - ly1) / (lx2 - lx1);
    float b = ly1 - k * lx1;

    float y_at_rx1 = k * rx1 + b;
    float y_at_rx2 = k * rx2 + b;

    float lnx_min = std::min(lx1, lx2);
    float lnx_max = std::max(lx1, lx2);

    if (rx1 >= lnx_min && rx1 <= lnx_max && y_at_rx1 >= ry1 && y_at_rx1 <= ry2 ||
        rx2 >= lnx_min && rx2 <= lnx_max && y_at_rx2 >= ry1 && y_at_rx2 <= ry2
    ) return true;

    if (std::abs(k) > delta) {
        float x_at_ry1 = (ry1 - b) / k;
        float x_at_ry2 = (ry2 - b) / k;

        float lny_min = std::min(ly1, ly2);
        float lny_max = std::max(ly1, ly2);

        if (ry1 >= lny_min && ry1 <= lny_max && x_at_ry1 >= rx1 && x_at_ry1 <= rx2) return true;
        if (ry2 >= lny_min && ry2 <= lny_max && x_at_ry2 >= rx1 && x_at_ry2 <= rx2) return true;
    }

    return false;

}

bool collision_checker::operator()(const shape &shp1, const shape &shp2) const {
    return std::visit(*this, shp1, shp2);
}
