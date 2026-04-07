#pragma once

#include "../../bytebitstream.h"
#include <string_view>

namespace lzss_ns {
    struct node {
        uint8_t pos = 0, len = 0;
        std::string data;
    };
    sstrtobb& operator<<(sstrtobb& os, const node& n);
    sstrtobb& operator>>(sstrtobb& is, node& n);
    void lzss_0(std::istream& stream_in, std::ostream& stream_out, const uint8_t& num_byte, size_t buffer_size);
    void de_lzss_0(std::istream& stream_in, std::ostream& stream_out, const uint8_t& num_byte, size_t buffer_size);
    // std::string de_lzss_0(const std::string& str, size_t buffer_size, const uint8_t& num_byte);
}

void lzss(std::istream& stream_in, std::ostream& stream_out, const uint8_t& num_byte);
void de_lzss(std::istream& stream_in, std::ostream& stream_out, const uint8_t& num_byte);
