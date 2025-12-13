#pragma once

#include "Header.h"

class Window {
protected:
    //WINDOW* parent;
    std::vector<Window*> vW;
    int posY = 0, posX = 0;
    bool isBox = false;
    bool isHide = false;

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

    const int& getSizeY(){return win->_maxy;}
    const int& getSizeX(){return win->_maxx;}
    const int& getPosY(){return posY;}
    const int& getPosX(){return posX;}

    virtual ~Window();
};
