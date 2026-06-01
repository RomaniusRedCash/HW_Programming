#pragma once

#include <cstdint>
#include <array>
#include <variant>
#include <SDL3/SDL.h>

class rectangle;
class circle;
class line;
class remove_obj;

using shape = std::variant<rectangle, line, circle, remove_obj>;

struct box_collision { float x, y, w, h; };

class object {
protected:
    std::array<uint8_t, 4> color = {0,0,0,255};
    box_collision box_col = {0,0,0,0};
public:
  void set_color(const std::array<uint8_t, 4>& color);
  virtual void render(SDL_Renderer *renderer) const = 0;
  void set_color(SDL_Renderer *renderer, const std::array<uint8_t, 4>& color) const;
  void set_color(SDL_Renderer *renderer) const;
  const box_collision &get_box_collision() const;
  virtual ~object() = default;
};

class remove_obj : public object{
public:
    void render(SDL_Renderer *renderer) const override {}
};


#include "rectangle/rectangle.h"
#include "circle/circle.h"
#include "line/line.h"
