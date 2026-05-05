#pragma once

#include <iostream>
#include <queue>
#include "../logger/logger.h"

extern std::queue<std::string> sub_commands;

namespace lz {
    struct node {
        size_t pos = 0;
        inline static size_t size = 1;
    };
    size_t get_window(const std::string& str, const size_t& def_num);
}

