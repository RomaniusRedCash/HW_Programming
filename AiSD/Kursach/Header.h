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
    game, setting, choose, quit, restart, finish
};

class InputHandler;
class MenuHandler;

extern uint8_t MaxNumNode;
extern uint8_t numNode;
extern std::vector<std::vector<HanoyNode>> vHanoys;
extern Sost nowSost;
extern int hod;
extern const uint8_t maxNode;
extern InputHandler inpH;
extern MenuHandler* wSetting;

