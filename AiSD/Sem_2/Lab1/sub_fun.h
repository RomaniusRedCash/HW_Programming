#pragma once

#include <string>
#include <iostream>

void RLE(std::istream& stream_in, std::ostream& stream_out);
void RLE2(std::istream& stream_in, std::ostream& stream_out);

void from_RLE(std::istream& stream_in, std::ostream& stream_out);

void enwik8_to_enwik(std::istream& stream_in, std::ostream& stream_out);

bool compare_f(std::istream& stream_in, std::istream& stream_in2);

