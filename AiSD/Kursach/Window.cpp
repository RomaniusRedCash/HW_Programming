#include "Window.h"

Window::Window(const int& sizeY, const int& sizeX) : sizeY(sizeY), sizeX(sizeX) {
    win = newwin(sizeY, sizeX, 0, 0);
    box(win,0,0);
    upDate();
}

void Window::move(const int& y, const int& x){
    posY = y;
    posX = x;
    mvwin(win, y, x);
    upDate();
}

void Window::upDate(){
    box(win,0,0);

    mvwprintw(win, 1, 2, "Window %dx%d", sizeY, sizeX);
    mvwprintw(win, 2, 2, "Pos: %d,%d", posY, posX);

    wrefresh(win);
}

void Window::hide(){
    isHide = true;
    werase(win);
    wrefresh(win);
}

void Window::show(){
    isHide = false;
    upDate();
}

void Window::moveCenter(const int& maxY, const int& maxX){
    this->move(maxY/2-sizeY/2, maxX/2-sizeX/2);
}

Window::~Window(){
    delwin(win);
}
