#include "GameWindow.h"
#include "InputHandler.h"
#include "ScreenHandler.h"
#include "MenuHandler.h"

//==============================================================================================

uint8_t numNode = 4;
const uint8_t MaxNumNode = 5, maxNode = MaxNumNode * 2 + 1;
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
    // setlocale(LC_ALL, "en_US.UTF-8");
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
    wSetting = new MenuHandler(winY, winX, wMain.win);
    wSetting->hide();
    wSetting->moveCenter(winY, winX);
    GameWindow* wGame = new GameWindow(winY, winX, wMain.win);
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

//void useKey() {
//    int ch = inpH.getCh();
//    
//}

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

void moveFromTo(const uint8_t& from, const uint8_t& to) {
    if (!vHanoys[to].empty() && vHanoys[to].back().size < vHanoys[from].back().size) throw HanErr();
    vHanoys[to].push_back(vHanoys[from].back());
    vHanoys[from].pop_back();
    hod++;
}

void moveAutoOne(const uint8_t& from, const uint8_t& to) {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    moveFromTo(from, to);
    FrameRate::newRate();
}


void moveAutoPair(const uint8_t& from, const uint8_t& to) {
    if (to == 3 && 
        (vHanoys[from].back().size == getMaxNowNode()
            || !vHanoys.back().empty()
            && vHanoys.back().back().size - vHanoys[from].back().size == 2)) {
        moveAutoOne(from, to - vHanoys[from].back().color % 2);
        moveAutoOne(from, to - vHanoys[from].back().color % 2);
        return;
    }
    moveAutoOne(from, to);
    moveAutoOne(from, to);
}

void autoSolve(const uint8_t& num, const uint8_t& from, const uint8_t& to) {
    uint8_t t1, t2;
    for (t1 = 0; t1 == from || t1 == to; ++t1 %= vHanoys.size());
    for (t2 = 0; t2 == from || t2 == to || t2 == t1; ++t2 %= vHanoys.size());

    std::function<void()> fPre = [] {};
    std::function<void()> fPost = [] {};

    switch (num)
    {
    case 2:
        break;
    case 4:
        fPre = [&from, &to, &t1, &t2, &num] {
            moveAutoPair(from, t1);
            };
        
        fPost = [&from, &to, &t1, &t2, &num] {
            moveAutoPair(t1, to);
            };
        break;
    case 6:
        if (to == 3) { // то же самое, что и case 8?
            fPre = [&from, &to, &t1, &t2, &num] {
                autoSolve(num - 2, from, t1);
                };
            fPost = [&from, &to, &t1, &t2, &num] {
                autoSolve(num - 2, t1, to);
                };
            break;
        }
        fPre = [&from, &to, &t1, &t2, &num] {
            moveAutoPair(from, t2);
            moveAutoPair(from, t1);
            };
        fPost = [&from, &to, &t1, &t2, &num] {
            moveAutoPair(t1, to);
            moveAutoPair(t2, to);
            };
        break;
    case 8:
        fPre = [&from, &to, &t1, &t2, &num] {
            autoSolve(num - 2, from, t1);
            };
        fPost = [&from, &to, &t1, &t2, &num] {
            autoSolve(num - 2, t1, to);
            };
        break;
    default:
        fPre = [&from, &to, &t1, &t2, &num] {
            autoSolve(num - 4, from, t2);
            moveAutoPair(from, t1);
            autoSolve(num - 4, t2, t1);
            };
        fPost = [&from, &to, &t1, &t2, &num] {
            autoSolve(num - 2, t1, to);
            };
        break;
    }


    fPre();
    moveAutoPair(from, to);
    fPost();
}
