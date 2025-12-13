#include "GameWindow.h"

void GameWindow::createWin(){
    if (nowSost == finish) {
        wFinish->show();
    }
    else if (nowSost != finish) {
        wFinish->hide();
    }
}

void GameWindow::addSidebar(Window* w) {
    if (vW.empty()) {
        w->move(1, 1);
    }
    else {
        Window*& lastW = vW.back();
        w->move(lastW->getPosY() + lastW->getSizeY(), 1);
    }
    addSub(w);
}

GameWindow::GameWindow(const int& sizeY, const int& sizeX, WINDOW* parent)
    : Window(sizeY, sizeX, parent) {

    isBox = true;

    TextWindow* wInstruction = new TextWindow(2, sizeX - 2, win);
    addSidebar(wInstruction);
    wInstruction->setText("←/→ or 1-4 - move, esc - menu.");
    wInstruction->setTextPosCenter();

    TowersWindow* wTower = new TowersWindow(11, sizeX - 2, win);
    addSidebar(wTower);

    PosWindow* wPos = new PosWindow(1, sizeX - 2, win);
    addSidebar(wPos);

    StatisticWindow* wStatistic = new StatisticWindow(1, sizeX - 2, win);
    addSidebar(wStatistic);

    wFinish = new StatisticWindow(6, getSizeX() - 2, win);
    wFinish->setText("You finish with %d hodov.\n Press esc for restart.");
    wFinish->setBox(true);
    wFinish->setTextPos(2,1);
    wFinish->moveCenter();
    addSub(wFinish);
}

GameWindow::~GameWindow() {

}
