#pragma once

#include "../object.h"

class line : public object {
public:
    float x1,y1,x2,y2;

    line(float x1, float y1, float x2, float y2);
    virtual void render(SDL_Renderer *renderer) const;
};
