#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <string_view>
#include <vector>
#include <set>
#include <algorithm>
#include "../../logger/logger.h"

namespace lzw_ns {
    struct node {
        size_t pos = 0;
        inline static uint8_t size = 1;
    };
    std::ostream& operator<<(std::ostream& os, const node& n);
    std::istream& operator>>(std::istream& is, node& n);
    std::string lzw_0(const std::string& str, size_t buffer_size, const uint8_t& num_byte, std::vector<std::string>& v_slovar);
    std::string de_lzw_0(const std::string& str, size_t buffer_size, const uint8_t& num_byte, std::vector<std::string>& v_slovar);
    std::string lzw_1(const std::string& str, size_t buffer_size, const uint8_t& num_byte);
    std::string lzw_2(const std::string& str, size_t buffer_size, const uint8_t& num_byte);
    std::string de_lzw_1(const std::string& str, size_t buffer_size, const uint8_t& num_byte);
    std::string de_lzw_2(const std::string& str, size_t buffer_size, const uint8_t& num_byte);
}

void lzw(std::istream& stream_in, std::ostream& stream_out, const uint8_t& num_byte);
