#pragma once

#include <ncurses.h>

class Window {
protected:
    int sizeY, sizeX;
    int posY = 0, posX = 0;
    bool isHide = false;
    WINDOW* win = nullptr;
public:
    Window(const int& sizeY, const int& sizeX);

    void move(const int& y, const int& x);

    virtual void upDate();
    void hide();

    void show();

    void moveCenter(const int& maxY, const int& maxX);

    const int& getSizeY(){return sizeY;}
    const int& getSizeX(){return sizeX;}

    virtual ~Window();
};
