#pragma once

#include <random>

class planer {
    mutable std::mt19937 gen;

    mutable std::uniform_real_distribution<float> dis_x;
    mutable std::uniform_real_distribution<float> dis_y;

    // float map_width;
    // float map_height;

public:
    planer(float width, float height) : /*map_width(width), map_height(height),*/  gen(std::random_device{}()), dis_x(0.0f, width), dis_y(0.0f, height) {}
    std::pair<float, float> get_random_point() const;
    void resize(float width, float height);
    std::mt19937 &get_gen();
};
