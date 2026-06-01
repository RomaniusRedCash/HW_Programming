#include "circle.h"

circle::circle(float x, float y, float r, bool fill)
: cx(x), cy(y), radius(r), is_fill(fill) {}

void circle::DrawCircle(SDL_Renderer *renderer, float cx, float cy, float radius) const {
  float x = radius;
  float y = 0.0f;
  float error = 1.0f - radius;
  while (x >= y) {
    SDL_RenderPoint(renderer, cx + x, cy + y);
    SDL_RenderPoint(renderer, cx + y, cy + x);
    SDL_RenderPoint(renderer, cx - y, cy + x);
    SDL_RenderPoint(renderer, cx - x, cy + y);
    SDL_RenderPoint(renderer, cx - x, cy - y);
    SDL_RenderPoint(renderer, cx - y, cy - x);
    SDL_RenderPoint(renderer, cx + y, cy - x);
    SDL_RenderPoint(renderer, cx + x, cy - y);
    y++;
    if (error < 0) {
      error += 2.0f * y + 1.0f;
    } else {
      x--;
      error += 2.0f * (y - x) + 1.0f;
    }
  }
}

void circle::DrawFillCircle(SDL_Renderer *renderer, float cx, float cy, float radius) const {
  float x = radius;
  float y = 0.0f;
  float error = 1.0f - radius;
  while (x >= y) {
    SDL_RenderLine(renderer, cx - x, cy + y, cx + x, cy + y);
    SDL_RenderLine(renderer, cx - y, cy + x, cx + y, cy + x);
    SDL_RenderLine(renderer, cx - x, cy - y, cx + x, cy - y);
    SDL_RenderLine(renderer, cx - y, cy - x, cx + y, cy - x);
    y++;
    if (error < 0) {
      error += 2.0f * y + 1.0f;
    } else {
      x--;
      error += 2.0f * (y - x) + 1.0f;
    }
  }
}

void circle::render(SDL_Renderer *renderer) const {
  set_color(renderer);
  if (is_fill) {
    DrawFillCircle(renderer, cx, cy, radius);
  } else {
    DrawCircle(renderer, cx, cy, radius);
  }
}
