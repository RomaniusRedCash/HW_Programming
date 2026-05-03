#pragma once

#include <cstdint>
#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <set>
#include <algorithm>
#include "../../logger/logger.h"
#include "../../bytebitstream.h"

extern uint8_t num_byte;

namespace lzw_ns {
    struct node {
        size_t pos = 0;
        inline static size_t size = 1;
    };
    std::vector<std::string> create_slovar_byte(const size_t& start = 0, size_t const& stop = 1 << (8 * num_byte));
    std::unordered_map<std::string, size_t> create_mapslovar_byte(const size_t& start = 0, size_t const& stop = 1 << (8 * num_byte));
    sstrtobb& operator<<(sstrtobb& os, node n);
    sstrtobb& operator>>(sstrtobb& is, node& n);
    sstrtobb lzw_0(const std::string& str, const size_t& buffer_size, std::unordered_map<std::string, size_t> m_slovar);
    std::string de_lzw_0(sstrtobb& ssbb, const size_t& buffer_size, std::vector<std::string> v_slovar);
    std::string lzw_1(const std::string& str, const size_t& buffer_size);
    // std::string lzw_2(const std::string& str, const size_t& buffer_size);
    // std::string de_lzw_1(const std::string& str, const size_t& buffer_size);
    // std::string de_lzw_2(const std::string& str, const size_t& buffer_size);
}

void lzw(std::istream& stream_in, std::ostream& stream_out, const size_t& buffer_size);
void de_lzw(std::istream& stream_in, std::ostream& stream_out, const size_t& buffer_size);
