#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <iostream>
#include <cstdint>

void RLE_bit(std::istream& inp_stream, std::ostream& out_stream, const int& ms, const int& mc);

class elmnt {
protected:
    int size = 8;
    std::string data;
public:
    elmnt(const std::string& str, const int& size);
    const int& get_size() const;
    const std::string& get_data() const;
    friend bool operator==(const elmnt& a, const elmnt& b);
};

class strelement {
    std::vector<elmnt> data;
    std::string str;
    int mc = 8, sdvig_buffer = 0;
    void sdvig_str(std::string space, const int& sdvig_byte);
public:
    strelement(const int& mc);
    strelement(const std::string& str, const int& start_point, const int& mc);
    const elmnt& operator[](const size_t& iter) const;
    void push_back(const elmnt& el);
    const std::string& burn_to_str(const int& ms);
};

class RLE_comp {
    size_t mask = 1, ms, mc;
    size_t num_char = 0;
    // elmnt last_elt;
    char start_point = 0;
public:
    RLE_comp (const int& ms, const int& mc);
    bool convert_str(std::istream& inp_stream, std::ostream& out_stream);
};

bool operator==(const elmnt& a, const elmnt& b);
