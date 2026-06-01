#include "rectangle.h"

rectangle::rectangle(float x, float y, float w, float h, bool is_fill) : rectangle({x,y,w,h}, is_fill) {}

rectangle::rectangle(const SDL_FRect& rectangle_data, bool is_fill) : is_fill(is_fill) {
    set_size(rectangle_data);
}

void rectangle::set_size(const SDL_FRect &rectangle_data) {
    this->rectangle_data = rectangle_data;
    box_col = {rectangle_data.x, rectangle_data.y, rectangle_data.w, rectangle_data.h};
}

void rectangle::render(SDL_Renderer *renderer) const {
    set_color(renderer);
    if (is_fill) {
        SDL_RenderFillRect(renderer, &rectangle_data);
    } else {
        SDL_RenderRect(renderer, &rectangle_data);
    }
}
