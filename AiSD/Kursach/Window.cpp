#include "Window.h"

void Window::createWin() {
    mvwprintw(win, 1, 2, "Window %dx%d", getSizeY(), getSizeX());
    mvwprintw(win, 2, 2, "Pos: %d,%d", posY, posX);
}

Window::Window(const int& sizeY, const int& sizeX, WINDOW* parent) {
    if (parent)
        win = derwin(parent, sizeY, sizeX, 0, 0);
    else win = newwin(sizeY, sizeX, 0, 0);
}

void Window::addSub(Window* win) {
    vW.push_back(win);
}

void Window::move(const int& y, const int& x){
    posY = y;
    posX = x;
    if (win->_parent)
        mvderwin(win, y, x);
    else mvwin(win, y, x);
    upDate();
}

void Window::moveCenter() {
    this->move(win->_parent->_maxy / 2 - getSizeY() / 2, win->_parent->_maxx / 2 - getSizeX() / 2);
}

void Window::moveCenter(const int& maxY, const int& maxX) {
    this->move(maxY / 2 - getSizeY() / 2, maxX / 2 - getSizeX() / 2);
}

void Window::setBox(const bool& isBox) {
    this->isBox = isBox;
    if (isBox) box(win, 0, 0);
}

void Window::upDate(){
    //std::this_thread::sleep_for(std::chrono::seconds(1));
    if (isHide) return;
    wclear(win);
    createWin();
    for (Window* w : vW)
        w->upDate();
    if (isBox) box(win, 0, 0);
    if (!win->_parent)
        wrefresh(win);
}

void Window::hide(){
    isHide = true;
    wclear(win);
}

void Window::show(){
    isHide = false;
}

Window::~Window(){
    for (Window* w : vW)
        delete w;
    delwin(win);
}
