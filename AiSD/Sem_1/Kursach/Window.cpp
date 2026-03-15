#include "Window.h"

void Window::createWin() {
    mvwprintw(win, 1, 2, "Window %dx%d", getSizeY(), getSizeX());
    mvwprintw(win, 2, 2, "Pos: %d,%d", posY, posX);
}

Window::Window(const int& sizeY, const int& sizeX, WINDOW* parent) {
#ifdef __unix__
    this->parent = parent;
    this->sizeY = sizeY;
    this->sizeX = sizeX;
#endif
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
    if (getParent())
        mvderwin(win, y, x);
    else mvwin(win, y, x);
}

void Window::moveCenter() {
    this->move(getmaxy(getParent()) / 2 - getSizeY() / 2, getmaxx(getParent()) / 2 - getSizeX() / 2);
}

void Window::moveCenter(const int& maxY, const int& maxX) {
    this->move(maxY / 2 - getSizeY() / 2, maxX / 2 - getSizeX() / 2);
}

void Window::setBox(const bool& isBox) {
    this->isBox = isBox;
    if (isBox) box(win, 0, 0);
}

void Window::setAutoClear(const bool& isAutoClear) {
    this->isAutoClear = isAutoClear;
}

void Window::upDate(){
    if (isHide) return; 
#ifdef __unix__
    wresize(win, sizeY, sizeX);
    move(posY, posX);
#endif
    if (isAutoClear) wclear(win);
    createWin();
    for (Window* w : vW)
        w->upDate();
    if (isBox) box(win, 0, 0);
    if (!getParent())
        wrefresh(win);
}

void Window::hide(){
    isHide = true;
}

void Window::show(){
    isHide = false;
}

WINDOW* Window::getParent() {
#ifdef _WIN32
    return win->_parent;
#elif __unix__
    return parent;
#endif

}
WINDOW* Window::getWin() {
    return win;
}

const int& Window::getSizeY() {
#ifdef _WIN32
    return win->_maxy;
#elif __unix__
    return sizeY;
#endif
}

const int& Window::getSizeX() {
#ifdef _WIN32
    return win->_maxx;
#elif __unix__
    return sizeX;
#endif
}

const int& Window::getPosY() {
    return posY;
}

const int& Window::getPosX() {
    return posX;
}

Window::~Window(){
    for (Window* w : vW)
        delete w;
    delwin(win);
}
