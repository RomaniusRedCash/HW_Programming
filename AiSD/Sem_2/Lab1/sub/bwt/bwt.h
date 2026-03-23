#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdint>
#include "../logger/logger.h"

namespace bwt_ns {
    std::string bwt_1(const std::string& str, const uint8_t& num_byte);
}

void bwt(std::istream& stream_in, std::ostream& stream_out, const uint8_t& num_byte);

void de_bwt(std::istream& stream_in, std::ostream& stream_out, const uint8_t& num_byte);
