#include "GameWindow.h"
#include "InputHandler.h"
#include "ScreenHandler.h"
#include "MenuHandler.h"

//==============================================================================================

const uint8_t maxNode = 11;
uint8_t numNode = 1;
uint8_t MaxNumNode = 5;
const int winY = 17, winX = maxNode * 4 + 7;
const uint8_t fps = 24;
Sost nowSost = game;

std::vector<std::vector<HanoyNode>> vHanoys(4);
int hod = 0;

size_t FrameRate::frame = 1;
std::vector<std::function<void(void)>> FrameRate::vTriger;

InputHandler inpH;
MenuHandler* wSetting;
//std::vector<std::string>

//==============================================================================================

void makeTower();
//void useKey();
bool isComplete();

//==============================================================================================

int main(){
    initscr();
    refresh();
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, true);
    resize_term(winY, winX);

    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);

    std::thread inputTh([] {inpH.init(); });
    
    for (std::vector<HanoyNode>& i : vHanoys) {
        i.reserve(numNode * 2);
    }
    makeTower();

    Window wMain(winY, winX);
    wSetting = new MenuHandler(winY, winX, wMain.win);
    wSetting->hide();
    wSetting->moveCenter(winY, winX);
    GameWindow* wGame = new GameWindow(winY, winX, wMain.win);
    //GameWindow wGame(winY, winX);
    wGame->moveCenter(winY, winX);

    //FrameRate::addTriger(&useKey);
    FrameRate::addTriger([] {if(isComplete()) nowSost = finish; });

    //ScreenHandler scrH(wGame);
    ScreenHandler scrH(wMain);
    scrH.addGameWin(wGame);
    scrH.addSettingWin(wSetting);
    FrameRate::addTriger([&scrH] {scrH.upDate(); });


    FrameRate::newRate();
    //std::thread scrTh([&scrH] {scrH.init(); });

    while (!quit) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        

    }





    inputTh.join();
    //scrTh.join();

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

//void useKey() {
//    int ch = inpH.getCh();
//    
//}

bool isComplete() {
    if (nowSost != game) return false;
    if (!vHanoys[0].empty() || !vHanoys[1].empty() || vHanoys[2].size() < numNode || vHanoys[3].size() < numNode) return false;
    for (uint8_t i = 2; i < vHanoys.size(); i++)
        for (uint8_t j = 0; j < vHanoys[i].size() - 1; j++)
            if (vHanoys[i][j].color != i - 1 || vHanoys[i][j].size - vHanoys[i][j + 1].size != 2) return false;
    return true;
}