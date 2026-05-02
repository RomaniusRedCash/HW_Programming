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
#include "../sub_fun.h"


namespace ha_ns {
    class calculator;

    class ha_code : public bytebit {
    public:
        ha_code();
        bool read_hacode(sstrtobb& ssbb, const calculator& calc, const size_t& max_size);
        bool operator<(const ha_code& hc) const;
        bool operator==(const ha_code& hc) const;
    };
}

namespace std {
    template <>
    struct hash<ha_ns::ha_code> {
        size_t operator()(const ha_ns::ha_code& b) const {
            return hash<std::string>{}(b.get_data());
        }
    };
}

namespace ha_ns {
    class node {
    protected:
        friend calculator;
        size_t num = 0, depth = 0;
        std::string data;
        node* left = nullptr, *right = nullptr;
    public:
        node(const size_t& num);
        node(const std::string& data, const size_t& num);
        bool operator<(const node& noda);
    };

    class calculator {
        bool read = false;
        std::vector<node*> v_node;
        // std::unordered_map<std::string, ha_code> mapa;
        bimap<std::string, ha_code> mapa;
        std::vector<std::string> v_data;
        // node *pivot;
        void order(node*& noda, ha_code& hc, const int8_t& balance);
        void zap(const std::string& sim, const ha_code& ha_c);
        void clear(node* n);
        void remap(const std::map<size_t, std::vector<std::string>>& mapa_in);
    public:
        // std::map<ha_code, std::string> de_mapa;

        calculator(const std::vector<node*>& v_node_in);
        calculator(const std::map<size_t, std::vector<std::string>>& mapa_in);
        const ha_code& operator[] (const std::string& sim) const;
        const std::string& operator[] (const ha_code& ha_c) const;
        bool have_hac (const ha_code& ha_c) const;
        std::unordered_map<std::string, const ha_code*>::const_iterator begin() const;
        std::unordered_map<std::string, const ha_code*>::const_iterator end() const;
        size_t get_size() const;
        const bimap<std::string, ha_code>& get_mapa() const;
        std::string print_model();
        const std::vector<std::string>&  get_ord() const;
    };
}
