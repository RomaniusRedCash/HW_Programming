#pragma once

#include "Window.h"
#include "SubWindowGame.h"



class GameWindow : public Window {
#ifdef _WIN32
    inline static char HELP[] = "←/→ or 1-4 - move, esc - menu.";
#elif __unix__
    inline static char HELP[] = "Arrow or 1-4 - move, esc - menu.";
#endif
    inline static char FINISH[] = "You finish with %d steps.\n Press esc for restart.";

    void createWin() override;
    StatisticWindow* wFinish = nullptr;
    void addSidebar(Window* w);
public:
    GameWindow(const int& sizeY, const int& sizeX, WINDOW* parent = nullptr);
    ~GameWindow();
};


