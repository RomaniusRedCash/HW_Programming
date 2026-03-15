#pragma once

#include <iostream>
#include <cstdint>
#include "../logger/logger.h"

void RLE(std::istream& stream_in, std::ostream& stream_out);

void from_RLE(std::istream& stream_in, std::ostream& stream_out);

void RLE2(std::istream& stream_in, std::ostream& stream_out, const int& num_byte);

void from_RLE2(std::istream& stream_in, std::ostream& stream_out, const int& num_byte);

std::string_view::const_iterator find_first_not_of_str(const std::string_view& str1, const std::string_view& str2);
