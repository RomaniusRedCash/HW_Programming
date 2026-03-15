#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include "../logger/logger.h"
#include "../bytebitstream.h"
#include <bitset>

namespace ha_ns {
    class ha_code : public bytebit {
    public:
        ha_code();
        void add_null();
        void add_one();
        void pop_back();
    };

    class calculator;
    class node {
    protected:
        friend calculator;
        size_t num = 0;
        std::string data;
        node* left = nullptr, *right = nullptr;
    public:
        node(const size_t& num);
        node(const std::string& data, const size_t& num);
        bool operator<(const node& noda);
    };

    class calculator {
        std::vector<node*> v_node;
        std::map<std::string, ha_code> mapa;
        // node *pivot;
        void order(node*& noda, ha_code& hc, const int8_t& balance);
    public:
        calculator(const std::vector<node*>& v_node_in);
        const ha_code& operator[] (const std::string& sim) const;
        const std::map<std::string, ha_code>::const_iterator begin() const;
        const std::map<std::string, ha_code>::const_iterator end() const;
    };

    void ha_1(std::istream& stream_in, std::ostream& stream_out, const int& num_byte, const calculator& calc);
}

void ha(std::istream& stream_in, std::ostream& stream_out, const int& num_byte);
