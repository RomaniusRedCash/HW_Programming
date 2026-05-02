#pragma once

#include <functional>
#include <iostream>
#include <string>
#include <algorithm>
#include <cstdint>
#include <string_view>
#include <thread>
#include <cstring>
#include <map>
#include <unordered_map>
#include "logger/logger.h"


void enwik8_to_enwik(std::istream& stream_in, std::ostream& stream_out);

template <typename Func>
void start_algorithm(const std::string& name, const Func& f) {
    logger() << "- start " << name << std::endl;
    f();
    logger() << "- stop " << name << std::endl;
}

enum eCOMMANDS : int;

template<typename T1, typename T2>
class bimap {
    std::unordered_map<T1, const T2*> map_12;
    std::unordered_map<T2, const T1*> map_21;
public:
    void insert(const T1& a, const T2& b) {
        typename std::unordered_map<T1, const T2*>::iterator pairse = map_12.insert({a,nullptr}).first;
        typename std::unordered_map<T2, const T1*>::iterator paires = map_21.insert({b,nullptr}).first;
        pairse->second = &paires->first;
        paires->second = &pairse->first;
    }
    const T2& operator[](const T1& a) const {
        return *map_12.at(a);
    }
    const T1& operator[](const T2& a) const {
        return *map_21.at(a);
    }
    typename std::unordered_map<T1, const T2*>::const_iterator beg12() const {
        return map_12.cbegin();
    }
    typename std::unordered_map<T1, const T2*>::const_iterator end12() const {
        return map_12.cend();
    }
    typename std::unordered_map<T2, const T1*>::const_iterator beg21() const {
        return map_21.cbegin();
    }
    typename std::unordered_map<T2, const T1*>::const_iterator end21() const {
        return map_21.cend();
    }
    size_t size() const {
        return map_12.size();
    }
    bool contain(const T2& a) const {
        return map_21.find(a) != map_21.end();
    }
    bool contain(const T1& a) const {
        return map_12.find(a) != map_12.end();
    }
    template<typename Func>
    void modify(T1 a, T2 b, const Func& f) {
        map_12.erase(a);
        map_21.erase(b);
        f(a,b);
        insert(a, b);
    }
};
