#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>
#include <algorithm>
#include "../../logger/logger.h"

namespace lz77_ns {
    struct node {
      size_t pos = 0, len =0;
      std::string next;
    };
    std::ostream& operator<<(std::ostream& os, const node& n);
    std::istream& operator>>(std::istream& is, node& n);
    std::string lz77_1(const std::string& str, size_t buffer_size, const uint8_t& num_byte);
    std::string de_lz77_1(const std::string& str, size_t buffer_size, const uint8_t& num_byte);
}

void lz77(std::istream& stream_in, std::ostream& stream_out, const uint8_t& num_byte);
