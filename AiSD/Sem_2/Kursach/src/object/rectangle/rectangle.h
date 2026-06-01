#pragma once

#include "../object.h"

class rectangle : public object {
    bool is_fill = true;
    SDL_FRect rectangle_data;
public:
  rectangle(float x, float y, float w, float h, bool is_fill = true);
  rectangle(const SDL_FRect& rectangle_data, bool is_fill = true);
  void set_size(const SDL_FRect &rectangle_data);
  void render(SDL_Renderer *renderer) const override;
};
