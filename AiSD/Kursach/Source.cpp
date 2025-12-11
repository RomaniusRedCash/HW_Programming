#include <ncurses.h>
#include <cstring>
#include <vector>
#include <chrono>
#include <thread>

#include "GameWindow.h"

#define SMALL_W "viewport is very small."

int winY = 20, winX = 50;

int main(){
    initscr();
    refresh();

    cbreak();
    noecho();
    keypad(stdscr, true);
    curs_set(false);

    // printw("_123456789_123456789_123456789_123456789_%d", std::strlen("_123456789_123456789_123456789_123456789_"));

    std::vector<std::vector<HanoyNode>> vHanoys;
    vHanoys.resize(4, std::vector<HanoyNode>(12));
    for (int8_t i = 1; i <= 5; i++){
        vHanoys[0][10-i*2] = {0, i * 2 + 1};
        vHanoys[0][10-i*2+1] = {1, i * 2 + 1};
    }

    // Window win(winY, winX);
    GameWindow win(winY, winX, vHanoys);

    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    win.moveCenter(maxY,maxX);

    while(true){
        if (wgetch(stdscr) == KEY_RESIZE){
            getmaxyx(stdscr, maxY, maxX);
            if (maxY < win.getSizeY() || maxX < win.getSizeX()) {
                win.hide();
                clear();
                int posX = maxX/2-std::strlen(SMALL_W)/2;
                posX = posX>0?posX:0;
                mvprintw(maxY/2,posX,SMALL_W);
            } else {
                clear();
                refresh();

                win.moveCenter(maxY,maxX);
                win.show();
                refresh();
            }
        }
        refresh();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    getch();
    endwin();
    return 0;
}
