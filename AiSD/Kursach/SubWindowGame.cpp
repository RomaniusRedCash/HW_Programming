#include "SubWindowGame.h"

void TowersWindow::createWin() {
    for (uint8_t i = 0; i < vHanoys.size(); i++) {
        wstandend(win);
        if (i > 1) wattrset(win, COLOR_PAIR(i - 1));
        int startX = i * maxNode + i + 1 + maxNode/2;
        for (uint8_t j = 0; j < maxNumNode * 2 + 1; j++) {
            mvwaddch(win, getSizeY() - 1 - j, startX, ACS_BLOCK);
        }
        for (uint8_t j = 0; j < vHanoys[i].size(); j++) {
            const HanoyNode& node = vHanoys[i][j];
            for (uint8_t k = 0; k < node.size; k++) {
                wattrset(win, COLOR_PAIR(node.color));
                //mvwaddch(win, max, i*maxNode + maxNode - vHanoys[i][j].size/2,);
                mvwaddch(win, getSizeY() - 1 - j, startX - node.size / 2 + k, ACS_BLOCK);
            }
        }
    }
    wattrset(win, A_NORMAL);
    //box(win, 0, 0);
    //wstandend(win);
}

TowersWindow::TowersWindow(const int& sizeY, const int& sizeX, WINDOW* win) : Window(sizeY, sizeX, win) {

}

void TextWindow::createWin() {
    if (isSelect)
        wbkgdset(win, COLOR_PAIR(3));
    else
        wbkgdset(win, A_NORMAL);
    wclear(win);
    mvwprintw(win, textPosY, textPosX, text.c_str());
}

void TextWindow::setTextPos(const int& y, const int& x) {
    textPosY = y;
    textPosX = x;
}

void TextWindow::setTextPosCenter() {
    textPosY = getSizeY() / 2 - (getSizeY() / 2) % 2;
    textPosX = 0;
}

void TextWindow::setSelect(const bool& isSelect) {
    this->isSelect = isSelect;
}

void PosWindow::createWin() {
    for (uint8_t i = 0; i < vHanoys.size(); i++) {
        wattrset(win, A_NORMAL);
        if (inpH.getGamePos() == i) wattrset(win, COLOR_PAIR(4));
        else if (inpH.getSelectGamePos() == i) wattrset(win, COLOR_PAIR(5));
        for (uint8_t j = 0; j < sizeSel; j++)
            mvwaddch(win, 0, i * maxNode + i + 1 + maxNode / 2 - sizeSel / 2 + j, ACS_HLINE);
    }
}

void StatisticWindow::createWin() {
    mvwprintw(win, textPosY, textPosX, text.c_str(), hod);
}

StatisticWindow::StatisticWindow(const int& sizeY, const int& sizeX, WINDOW* win) : TextWindow(sizeY, sizeX, win) { 
    text = "hodov: %d"; 
}

void OptionsWindow::createWin() {
    for (int8_t i = 0; i < vW.size(); i++) {
        if (i == nowSelect())
            static_cast<TextWindow*>(vW[i])->setSelect(true);
        else
            static_cast<TextWindow*>(vW[i])->setSelect(false);
        vW[i]->move(i, 0);
    }
        
    //for (Window* w : vW) w->upDate();
}

OptionsWindow::OptionsWindow(const int& sizeY, const int& sizeX, WINDOW* win) : Window(sizeY, sizeX, win) {

}

void OptionsWindow::init() {
    WINDOW* parent = nullptr;
    if (win) {
        parent = getParent();
        for (Window* w : vW) delete w;
        sizeX = getSizeX();
        delwin(win);
    }
    win = derwin(parent, vSetStr.size(), sizeX, 0 ,0);
#ifdef __unix__
    sizeY = vSetStr.size();
#endif

    for (int i = 0; i < vSetStr.size(); i++) {
        TextWindow* wT = new TextWindow(1, getSizeX(), win);
        wT->setText(vSetStr[i]);
        addSub(wT);
    }
}

void OptionsWindow::addOption(const std::string& str) {
    vSetStr.push_back(str);
}

int8_t OptionsWindow::nowSelect() {
    return inpH.getVerticalPos() % vSetStr.size();
}

OptionsWindow::~OptionsWindow() {
}
