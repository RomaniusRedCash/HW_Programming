#pragma once

#include "Window.h"
#include "SubWindowGame.h"



class GameWindow : public Window {
    void createWin() override;
    StatisticWindow* wFinish = nullptr;
    void addSidebar(Window* w);
public:
    GameWindow(const int& sizeY, const int& sizeX, WINDOW* parent = nullptr);
    ~GameWindow();
};


