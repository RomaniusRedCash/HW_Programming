#pragma once

#include <cstdint>
#include <vector>
#include <unordered_map>
#include "itu_ha_classes.h"
#include "../bytebitstream.h"

namespace itu_ns {
    uint8_t get_category(const int32_t& x);
    void write_shift(sstrtobb& ssbb, const uint8_t& size, const int32_t& x);
    void read_shift(sstrtobb& ssbb, const uint8_t& size);
    enum e_layer : uint8_t {
        eCHROM, eNOCHROM
    };
    bool try_read_code(sstrtobb& ssbb, bytebit& bb, const calculator& calc);
    e_layer get_layer(const std::string& str);
}

void itu(std::istream& stream_in, std::ostream& stream_out, const itu_ns::e_layer& layer);

void de_itu(std::istream& stream_in, std::ostream& stream_out, const itu_ns::e_layer& layer);
