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
    upDate();
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

void Window::upDate(){
    //std::this_thread::sleep_for(std::chrono::seconds(1));
    if (isHide) return;
#ifdef __unix__
    wresize(win, sizeY, sizeX);
#endif
    wclear(win);
    // mvprintw(5,5,"nothide");
    createWin();
    for (Window* w : vW)
        w->upDate();
    if (isBox) box(win, 0, 0);
    if (!getParent())
        wrefresh(win);
}

void Window::hide(){
    isHide = true;
    wclear(win);
}

void Window::show(){
    isHide = false;
}

WINDOW* Window::getParent(){
    return parent;
}
WINDOW* Window::getWin(){
    return win;
}

int Window::getSizeY() {
#ifdef __WIN32__
    return win->_maxy;
#elif __unix__
    return sizeY;
#endif
}

int Window::getSizeX() {
#ifdef __WIN32__
    return win->_maxy;
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
