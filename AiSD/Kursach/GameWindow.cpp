#include "GameWindow.h"

GameWindow::GameWindow(const int& sizeY, const int& sizeX, const std::vector<std::vector<HanoyNode>>& vHanoys) : vHanoys(vHanoys), Window(sizeY, sizeX) {
    init_pair(0, COLOR_BLUE, COLOR_BLACK);
    init_pair(1, COLOR_RED, COLOR_BLACK);
}


void GameWindow::upDate(){
    for (uint8_t i = 0; i < vHanoys.size(); i++){
        for (uint8_t j = 0; j < vHanoys[i].size(); j++){
            for (uint8_t k = 0; k < vHanoys[i][j].size; k++)
                mvwaddch(win, sizeY-j-2, i*9+i+2 + (9 - vHanoys[i][j].size)/2+k, ACS_BLOCK);
            mvwaddch(win, sizeY-j-2, i*9+i+2+9/2, ACS_BLOCK);
        }
    }
    box(win,0,0);
    wrefresh(win);
}
