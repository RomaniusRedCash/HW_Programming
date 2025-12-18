#include "GameWindow.h"
#include "InputHandler.h"
#include "ScreenHandler.h"
#include "MenuHandler.h"

//==============================================================================================

uint8_t numNode = 1;
const uint8_t maxNumNode = 5, maxNode = maxNumNode * 2 + 1;
const int winY = maxNumNode * 2 + 7, winX = maxNode * 4 + 7;
const uint8_t fps = 24;
Sost nowSost = game;

std::vector<std::vector<HanoyNode>> vHanoys(4);
int hod = 0;

InputHandler inpH;
MenuHandler* wSetting;

//==============================================================================================

int main(){
    initscr();
    refresh();
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, true);
#ifdef _WIN32
    resize_term(winY, winX);
#elif __unix__
    set_escdelay(10);
#endif

    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, 8);
    init_pair(4, COLOR_GREEN, COLOR_BLACK);
    init_pair(5, COLOR_GREEN, 8);

    for (std::vector<HanoyNode>& i : vHanoys) {
        i.reserve(numNode * 2);
    }
    makeTower();

    Window wMain(winY, winX);
    wSetting = new MenuHandler(winY, winX, wMain.getWin());
    wSetting->hide();
    wSetting->moveCenter(winY, winX);
    GameWindow* wGame = new GameWindow(winY, winX, wMain.getWin());
    wGame->moveCenter(winY, winX);

    FrameRate::addTriger([] {if(isComplete()) nowSost = finish; });

    ScreenHandler scrH(wMain);
    scrH.addGameWin(wGame);
    scrH.addSettingWin(wSetting);
    FrameRate::addTriger([&scrH] {scrH.upDate(); });

    FrameRate::newRate();
    inpH.init();

    endwin();
    return 0;
}

//==============================================================================================

void makeTower() {
    for (std::vector<HanoyNode>& i : vHanoys)
        i.clear();
    for (uint8_t i = 0; i < numNode; i++) {
        vHanoys[0].emplace_back((numNode - i) * 2 + 1, 1);
        vHanoys[0].emplace_back((numNode - i) * 2 + 1, 2);
    }
}

uint8_t getMaxNowNode() {
    return numNode * 2 + 1;
}

bool isComplete() {
    if (nowSost != game) return false;
    if (!vHanoys[0].empty() || !vHanoys[1].empty() || vHanoys[2].size() < numNode || vHanoys[3].size() < numNode) return false;
    for (uint8_t i = 2; i < vHanoys.size(); i++)
        for (uint8_t j = 0; j < vHanoys[i].size() - 1; j++)
            if (vHanoys[i][j].color != i - 1 || vHanoys[i][j].size - vHanoys[i][j + 1].size != 2) return false;
    return true;
}

//==============================================================================================

void moveFromTo(const uint8_t& from, const uint8_t& to) {
    if (!vHanoys[to].empty() && vHanoys[to].back().size < vHanoys[from].back().size || to >= vHanoys.size()) throw HanErr();
    vHanoys[to].push_back(vHanoys[from].back());
    vHanoys[from].pop_back();
    hod++;
}

void moveAutoOne(const uint8_t& from, const uint8_t& to) {
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    //getch();
    moveFromTo(from, to);
    FrameRate::newRate();
}


void moveAutoPair(const uint8_t& from, const uint8_t& to) {
    moveAutoOne(from, to);
    moveAutoOne(from, to);
}

void moveAutoEndOne(const uint8_t& from) {
    moveAutoOne(from, vHanoys.size() - 1 - vHanoys[from].back().color % 2);
}

void moveAutoEndPair(const uint8_t& from) {
    moveAutoEndOne(from);
    moveAutoEndOne(from);
}


void autoSolve(const uint8_t& num, const uint8_t& from, const uint8_t& to, const bool& isFinal) {
    if (num == 0) return;
    uint8_t t1, t2, t3;
    for (t1 = 0; t1 == from || t1 == to; ++t1 %= vHanoys.size());
    for (t2 = 0; t2 == from || t2 == to || t2 == t1; ++t2 %= vHanoys.size());
    if (isFinal) {
        switch (num)
        {
        case 1:
            moveAutoEndPair(from);
            break;
        case 2:
            autoSolve(1, from, t1, false);
            autoSolve(1, from, to, isFinal);
            autoSolve(1, t1, to, isFinal);
            break;
        default:
            autoSolve(num - 2, from, t1, false);
            t2 = vHanoys.size() - 1 - vHanoys[from][1].color % 2;
            t3 = vHanoys.size() - t2 + 1;
            autoSolve(1, from, t3, false);
            autoSolve(1, from, t2, false);
            autoSolve(1, t3, from, false);
            moveAutoOne(t2, t3);
            autoSolve(1, from, to, isFinal);
            autoSolve(num - 2, t1, to, isFinal);
            break;
        }
    }
    else
        switch (num)
        {
        case 1:
            moveAutoPair(from, to);
            break;
        default:
            autoSolve(num - 2, from, t1, isFinal);
            autoSolve(1, from, t2, isFinal);
            autoSolve(1, from, to, isFinal);
            autoSolve(1, t2, to, isFinal);
            autoSolve(num - 2, t1, to, isFinal);
            break;
        }
}
