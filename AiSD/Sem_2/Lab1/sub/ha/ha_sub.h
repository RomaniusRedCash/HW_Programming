#pragma once

#include "ha.h"

namespace ha_ns {
    void write_shift_size(std::ostream& stream_out, const size_t& size);
    void write_shift_size(std::ostream& stream_out, const calculator& calc);

    void read_shift_size(std::istream& stream_in, size_t& size);
    void read_shift_size(std::istream& stream_in, std::map<size_t, std::vector<std::string>>& mapa, const size_t& model_size, const uint8_t num_byte, const uint8_t& node_size);
}
