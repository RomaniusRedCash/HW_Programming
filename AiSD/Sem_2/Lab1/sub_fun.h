#pragma once

#include <iostream>
#include <string>
#include <algorithm>
// #include <cstdint>
#include <string_view>
#include <thread>
// #include <cstddef>
#include <cstring>

void RLE(std::istream& stream_in, std::ostream& stream_out);

void from_RLE(std::istream& stream_in, std::ostream& stream_out);

void enwik8_to_enwik(std::istream& stream_in, std::ostream& stream_out);

bool compare_f(std::istream& stream_in, std::istream& stream_in2);
/*
template<typename T1, typename T2, typename T3>
class triple : public std::pair<T1, T2> {
public:
    T3 third;
    triple (const T1& t1, const T2& t2, const T3& t3) : std::pair<T1, T2>(t1,t2), third(t3) {};
};*/


void RLE2(std::istream& stream_in, std::ostream& stream_out, const int& num_byte);

void from_RLE2(std::istream& stream_in, std::ostream& stream_out, const int& num_byte);

std::string_view::const_iterator find_first_not_of_str(const std::string_view& str1, const std::string_view& str2);
