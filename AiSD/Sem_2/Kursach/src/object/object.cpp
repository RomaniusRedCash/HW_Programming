#include "object.h"

void object::set_color(const std::array<uint8_t, 4>& color) {
    this->color = color;
}

void object::set_color(SDL_Renderer *renderer, const std::array<uint8_t, 4>& color) const {
    SDL_SetRenderDrawColor(renderer, color[0], color[1], color[2], color[3]);
}

void object::set_color(SDL_Renderer *renderer) const {
    set_color(renderer, color);
}

const box_collision &object::get_box_collision() const { return box_col; };
