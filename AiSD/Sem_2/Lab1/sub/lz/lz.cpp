#include "lz.h"

size_t lz::get_window(const std::string& str, const size_t& def_num) {
    int window = 0;
    if (sub_commands.front().empty()) return def_num;
    try {
        window = std::stoi(str);
    } catch (...) {
        logger() << "unknown parametrs: "<<sub_commands.front()<<std::endl;
        return def_num;
    }
    if (window == 0 || window < 0) {
        window = def_num;
        logger() << "ERROR! illegal size "<<window<<". Change to " <<def_num << std::endl;
    }
    return window;
}
