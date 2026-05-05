#pragma once

#include <string_view>
#include <queue>
#include "../../bytebitstream.h"
#include "../lz.h"

extern uint8_t num_byte;
extern std::queue<std::string> sub_commands;

namespace lzss_ns {
    struct node : public lz::node{
        size_t len = 0;
        std::string data;
    };
    sstrtobb& operator<<(sstrtobb& os, const node& n);
    sstrtobb& operator>>(sstrtobb& is, node& n);
    void lzss_1(std::istream& stream_in, std::ostream& stream_out, const size_t window_buffer_size);
    void de_lzss_1(std::istream& stream_in, std::ostream& stream_out, const size_t window_buffer_size);
    // std::string de_lzss_0(const std::string& str, size_t buffer_size, const uint8_t& num_byte);
    bool try_read_node(sstrtobb& ssbb_in, node& n);
}

void lzss(std::istream& stream_in, std::ostream& stream_out);
void de_lzss(std::istream& stream_in, std::ostream& stream_out);
