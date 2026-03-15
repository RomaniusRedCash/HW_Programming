#pragma once

#include <iostream>
#include <string>
#include <algorithm>
#include <cstdint>
#include <string_view>
#include <thread>
// #include <cstddef>
#include <cstring>


void enwik8_to_enwik(std::istream& stream_in, std::ostream& stream_out);

bool compare_f(std::istream& stream_in, std::istream& stream_in2);
