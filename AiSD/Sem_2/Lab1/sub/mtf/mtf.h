#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <cstdint>
#include <vector>
#include <deque>
#include <algorithm>
#include "../logger/logger.h"

void mtf(std::istream& stream_in, std::ostream& stream_out, const int& num_byte);
void de_mtf(std::istream& stream_in, std::ostream& stream_out, const int& num_byte);
