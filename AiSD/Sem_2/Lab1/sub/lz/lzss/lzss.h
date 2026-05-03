#pragma once

#include "../../bytebitstream.h"
#include <string_view>

extern uint8_t num_byte;
extern size_t buffer_size_lz;

namespace lzss_ns {
    struct node {
        size_t pos = 0, len = 0;
        std::string data;
        inline static size_t size_len = 1;
    };
    sstrtobb& operator<<(sstrtobb& os, const node& n);
    sstrtobb& operator>>(sstrtobb& is, node& n);
    void lzss_1(std::istream& stream_in, std::ostream& stream_out);
    void de_lzss_1(std::istream& stream_in, std::ostream& stream_out);
    // std::string de_lzss_0(const std::string& str, size_t buffer_size, const uint8_t& num_byte);
    bool try_read_node(sstrtobb& ssbb_in, node& n);
}

void lzss(std::istream& stream_in, std::ostream& stream_out);
void de_lzss(std::istream& stream_in, std::ostream& stream_out);
