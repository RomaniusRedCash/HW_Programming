#include "planer.h"

std::pair<float, float> planer::get_random_point() const {
    return std::pair<float, float>{dis_x(gen), dis_y(gen)};
}

void planer::resize(float width, float height) {
    if (dis_x.max() != width || dis_y.max() != height) {
        dis_x = std::uniform_real_distribution<float>(0.0f, width);
        dis_y = std::uniform_real_distribution<float>(0.0f, height);
    }
}
std::mt19937 &planer::get_gen() { return gen; }
