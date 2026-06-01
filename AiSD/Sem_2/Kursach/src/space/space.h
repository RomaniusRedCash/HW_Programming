#pragma once

#include <vector>
#include "../object/object.h"

class space {
    std::vector<const shape*> v_objects;

public:
    space() = default;
    bool line_collision(float x1, float y1, float x2, float y2) const;
    void add_object(const shape *shp);
    void clear();
    bool del(size_t id);
};
