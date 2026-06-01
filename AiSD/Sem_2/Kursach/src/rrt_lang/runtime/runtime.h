#pragma once

#include "../parser/parser.h"

class runtime {
public:
    static void run(const std::vector<std::unique_ptr<command>>& commands, world &wrd);
};
