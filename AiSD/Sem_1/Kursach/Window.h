#pragma once

#include "Header.h"

class Window {
private:
    bool isBox = false;
    bool isHide = false;
    bool isAutoClear = true;
    int posY = 0, posX = 0;

protected:
#ifdef __unix__
    int sizeY, sizeX;
    WINDOW* parent;
#endif
    WINDOW* win = nullptr;
    std::vector<Window*> vW;
    virtual void createWin();

public:
    Window(const int& sizeY, const int& sizeX, WINDOW* parent = nullptr);
    void addSub(Window* win);

    void move(const int& y, const int& x);
    void moveCenter();
    void moveCenter(const int& maxY, const int& maxX);

    void setBox(const bool& isBox);
    void setAutoClear(const bool& isAutoClear);
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
