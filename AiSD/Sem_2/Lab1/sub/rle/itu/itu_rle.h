#pragma once

#include <cstdint>
#include <vector>
#include <unordered_map>
#include "itu_ha_classes.h"
#include "../../bytebitstream.h"
// #include "../../ha/ha_classes.h"

namespace itu_ns {
    uint8_t get_category(const int32_t& x);
    void write_shift(sstrtobb& ssbb, const uint8_t& size, const int32_t& x);
    enum e_layer : uint8_t {
        eCHROM, eNOCHROM
    };
}

void itu_rle(std::istream& stream_in, std::ostream& stream_out, const itu_ns::e_layer& layer);

void itu_de_rle(std::istream& stream_in, std::ostream& stream_out);
