#include "space.h"

#include "../object/line/line.h"
#include "collision_checker/collision_checker.h"

bool space::line_collision(float x1, float y1, float x2, float y2) const {
    shape ln = line(x1, y1, x2, y2);
    collision_checker cc;
    for (const shape *obj : v_objects) {
        if (obj && cc(*obj, ln))
            return true;
    }
    return false;
}
void space::add_object(const shape *shp) {
    v_objects.push_back(shp);
}

void space::clear() {
    v_objects.clear();
}

bool space::del(size_t id) {
    if (id < v_objects.size()) {
        v_objects[id] = nullptr;
        return true;
    }
    return false;
}
