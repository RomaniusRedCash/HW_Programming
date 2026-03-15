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
    setAutoClear(false);
    setBox(true);

    TextWindow* wInstruction = new TextWindow(2, sizeX - 2, getWin());
    addSidebar(wInstruction);
    std::string str;
    wInstruction->setText(HELP);
    wInstruction->setTextPosCenter();

    TowersWindow* wTower = new TowersWindow(maxNumNode * 2 + 1, sizeX - 2, getWin());
    addSidebar(wTower);

    PosWindow* wPos = new PosWindow(1, sizeX - 2, getWin());
    addSidebar(wPos);

    StatisticWindow* wStatistic = new StatisticWindow(1, sizeX - 2, getWin());
    addSidebar(wStatistic);

    wFinish = new StatisticWindow(6, getSizeX() - 2, getWin());
    wFinish->setText(FINISH);
    wFinish->setBox(true);
    wFinish->setTextPos(2,1);
    wFinish->moveCenter();
    addSub(wFinish);
}

GameWindow::~GameWindow() {

}
