#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <queue>
#include <bitset>
#include <cstdint>
#include <algorithm>
#include "../logger/logger.h"
#include "../bytebitstream.h"
#include "ha_classes.h"

namespace ha_ns {
    void ha_1(std::istream& stream_in, std::ostream& stream_out, const uint8_t& num_byte, const calculator& calc);
}

void ha(std::istream& stream_in, std::ostream& stream_out, const uint8_t& num_byte);

void de_ha(std::istream& stream_in, std::ostream& stream_out, const uint8_t& num_byte);

