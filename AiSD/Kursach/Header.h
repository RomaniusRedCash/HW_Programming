#pragma once

#include <curses.h>
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
extern const uint8_t MaxNumNode, maxNode;
extern std::vector<std::vector<HanoyNode>> vHanoys;
extern Sost nowSost;
extern int hod;
extern InputHandler inpH;
extern MenuHandler* wSetting;

void moveFromTo(const uint8_t& from, const uint8_t& to);
void autoSolve(const uint8_t& num, const uint8_t& from, const uint8_t& to);

class HanErr {
};
