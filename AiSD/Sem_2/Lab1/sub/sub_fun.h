#pragma once

#include <functional>
#include <iostream>
#include <string>
#include <algorithm>
#include <cstdint>
#include <string_view>
#include <thread>
#include <cstring>
#include <unordered_map>
#include "logger/logger.h"


void enwik8_to_enwik(std::istream& stream_in, std::ostream& stream_out);

bool compare_f(std::istream& stream_in, std::istream& stream_in2);

template <typename Func>
void start_algorithm(const std::string& name, const Func& f) {
    logger() << "- start " << name << std::endl;
    f();
    logger() << "- stop " << name << std::endl;
}

enum eCOMMANDS : int;

template<typename T1, typename T2>
class bimap {
    std::unordered_map<T1, const T2*> map_s_e;
    std::unordered_map<T2, const T1*> map_e_s;
public:
    void insert(const T1& a, const T2& b) {
        typename std::unordered_map<T1, const T2*>::iterator pairse = map_s_e.insert({a,nullptr}).first;
        typename std::unordered_map<T2, const T1*>::iterator paires = map_e_s.insert({b,nullptr}).first;
        pairse->second = &paires->first;
        paires->second = &pairse->first;
    }
    const T2& operator[](const T1& a) const {
        return *map_s_e.at(a);
    }
    const T1& operator[](const T2& a) const {
        return *map_e_s.at(a);
    }
};
