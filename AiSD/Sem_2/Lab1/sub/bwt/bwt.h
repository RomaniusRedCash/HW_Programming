#pragma once

#include <iostream>
#include <string_view>
#include <string>
#include <cstring>
#include <vector>
#include <array>
#include <algorithm>
#include <limits>
#include <numeric>
#include <cstdint>
#include <map>
#include <unordered_map>
#include <deque>
#include "../logger/logger.h"
#include "../lz/lz.h"

namespace bwt_ns {
    std::string bwt_1(const std::string& str);
    std::string bwt_2(const std::string& str);
    std::string de_bwt_1(const std::string& s_bwt, const uint8_t& bwt_pos);
    std::string de_bwt_2(const std::string& s_bwt, const uint32_t& bwt_pos);
    std::string de_bwt_3(const std::string& s_bwt, const uint8_t& bwt_pos);
    std::string de_bwt_0(const std::string& s_bwt);
}

void bwt(std::istream& stream_in, std::ostream& stream_out);

void de_bwt(std::istream& stream_in, std::ostream& stream_out);
