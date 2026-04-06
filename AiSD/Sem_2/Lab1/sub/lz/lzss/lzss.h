#pragma once

#include "../../bytebitstream.h"

namespace lzss_ns {
    std::string lzss_0(const std::string& str, size_t buffer_size, const uint8_t& num_byte);
    std::string de_lzss_0(const std::string& str, size_t buffer_size, const uint8_t& num_byte);
}

void lzss(std::istream& stream_in, std::ostream& stream_out, const uint8_t& num_byte);
void de_lzss(std::istream& stream_in, std::ostream& stream_out, const uint8_t& num_byte);
