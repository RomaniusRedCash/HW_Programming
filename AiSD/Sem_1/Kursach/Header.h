#pragma once

#ifdef __unix__
#include <ncurses.h>
#elif _WIN32
#include <curses.h>
#endif

#include <cstring>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <ctime>
#include <functional>

struct HanoyNode {
    uint8_t size;
    uint8_t color;
    HanoyNode(uint8_t size, uint8_t color) : size(size), color(color){
    }
};

enum Sost : int8_t {
    game, choose, restart, autoS, quit, setting, finish
};

class InputHandler;
class MenuHandler;

extern uint8_t numNode, maxNowNode;
extern const uint8_t maxNumNode, maxNode;
extern std::vector<std::vector<HanoyNode>> vHanoys;
extern Sost nowSost;
extern int hod;
extern InputHandler inpH;
extern MenuHandler* wSetting;

void makeTower();
bool isComplete();
void moveFromTo(const uint8_t& from, const uint8_t& to);
void autoSolve(const uint8_t& num, const uint8_t& from, const uint8_t& to, const bool& isFinal);

class HanErr {
};
