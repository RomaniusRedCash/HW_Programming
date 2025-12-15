#pragma once

#include "Header.h"

class Window {
protected:
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
    WINDOW* win = nullptr;

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

    int getSizeY();
    int getSizeX();
    const int& getPosY();
    const int& getPosX();

    virtual ~Window();
};
