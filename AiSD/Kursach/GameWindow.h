#pragma once

#include <deque>
#include <vector>

#include "Window.h"

struct HanoyNode {
    bool isColor;
    uint8_t size;
};

class GameWindow : public Window {
    const std::vector<std::vector<HanoyNode>>& vHanoys;
public:
    GameWindow(const int& sizeY, const int& sizeX, const std::vector<std::vector<HanoyNode>>& vHanoys);
    void upDate() override;
};
