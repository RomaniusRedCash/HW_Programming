#pragma once

#include <cstdint>
#include <iostream>
#include <limits>
#include <string>
#include <cstring>
#include <sstream>
#include <string_view>
#include <vector>
#include <queue>
#include <unordered_map>
#include <set>
#include <algorithm>
#include "../lz.h"
#include "../../logger/logger.h"
#include "../../bytebitstream.h"

extern uint8_t num_byte;
extern std::queue<std::string> sub_commands;

namespace lzw_ns {
    enum lzw_style : uint8_t {
        defaultlzw, full
    };
    struct node : public lz::node {
    };
    struct init_data {
        size_t size_slovar = std::numeric_limits<size_t>::max();
        lzw_style style = defaultlzw;
    };
    init_data read_param();
    // std::pair<size_t, size_t> calc_maxmin(const std::string& ss);
    std::vector<std::string> create_slovar_byte();
    std::vector<std::string> create_slovar_byte(std::istream& is);
    std::unordered_map<std::string, size_t> create_mapslovar_byte();
    std::unordered_map<std::string, size_t> create_mapslovar_byte(std::istream& is, std::ostream& os);
    sstrtobb& operator<<(sstrtobb& os, node n);
    sstrtobb& operator>>(sstrtobb& is, node& n);
    sstrtobb lzw_0(const std::string& str, std::unordered_map<std::string, size_t>& m_slovar, const init_data& idata);
    std::string de_lzw_0(sstrtobb& ssbb, std::vector<std::string>& v_slovar, const init_data& idata);
    std::string lzw_1(const std::string& str, const init_data& idata);
}

void lzw(std::istream& stream_in, std::ostream& stream_out);
void de_lzw(std::istream& stream_in, std::ostream& stream_out);
