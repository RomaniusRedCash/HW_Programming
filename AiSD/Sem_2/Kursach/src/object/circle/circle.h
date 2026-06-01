#pragma once

#include <SDL3/SDL.h>

#include "../object.h"

class circle : public object {
private:
    float cx, cy;
    float radius;
    bool is_fill;

    void DrawCircle(SDL_Renderer *renderer, float cx, float cy, float radius) const;
    void DrawFillCircle(SDL_Renderer *renderer, float cx, float cy, float radius) const;

  public:
    circle(float x, float y, float r, bool fill = true);
    void render(SDL_Renderer *renderer) const override;
};
