#pragma once

#include "Header.h"

class Window {
protected:
    WINDOW* win = nullptr;
    std::vector<Window*> vW;
    int posY = 0, posX = 0;
    bool isBox = false;
    bool isHide = false;

#ifdef __unix__
    WINDOW* parent;
    int sizeY, sizeX;
#endif

    virtual void createWin();
public:

    Window(const int& sizeY, const int& sizeX, WINDOW* parent = nullptr);
    void addSub(Window* win);

    void move(const int& y, const int& x);
    void moveCenter();
    void moveCenter(const int& maxY, const int& maxX);

    void setBox(const bool& isBox);
    void upDate();
    void hide();
    void show();

    WINDOW* getParent();
    WINDOW* getWin();

    const int& getSizeY();
    const int& getSizeX();
    const int& getPosY();
    const int& getPosX();

    virtual ~Window();
};
