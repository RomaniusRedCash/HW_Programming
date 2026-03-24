#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>
#include <algorithm>
#include "../../logger/logger.h"

namespace lzw_ns {
    struct node {
        size_t pos = 0;
        size_t next;
    };
    std::ostream& operator<<(std::ostream& os, const node& n);
    std::istream& operator>>(std::istream& is, node& n);
    std::string lzw_1(const std::string& str, size_t buffer_size, const uint8_t& num_byte);
    std::string de_lzw_1(const std::string& str, size_t buffer_size, const uint8_t& num_byte);
}

void lzw(std::istream& stream_in, std::ostream& stream_out, const uint8_t& num_byte);
