#include "line.h"

#include <cmath>

line::line(float x1, float y1, float x2, float y2)
    : x1(x1), y1(y1), x2(x2), y2(y2) {
    box_col = {x1, y1, x2 - x1, y2 - y1};
}
void line::render(SDL_Renderer *renderer) const {
    set_color(renderer);
    SDL_RenderLine(renderer, x1,y1,x2,y2);
}
