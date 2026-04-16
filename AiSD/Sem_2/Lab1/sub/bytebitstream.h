#pragma once

#include <iostream>
#include <string>
#include <cinttypes>
#include "../sub/logger/logger.h"
#include <bitset>
#include "logger/logger.h"
#include <cstdint>

class sstrtobb;

class ssbb_base {
protected:
    std::string data;
    int8_t buffer_sdvig_size = 0;
public:
    void sdvig(int8_t delta);
    ssbb_base() = default;
    ssbb_base(const std::string& str);
    virtual const std::string& get_data() const = 0;
    const int8_t& real_last() const;
    const int8_t& get_buffer_sdvig_size() const;
    void set_buffer_sdvig_size(const int8_t& sdvig);
    virtual ~ssbb_base() = default;
};


class bytebit : public ssbb_base{
protected:
    friend sstrtobb;
    size_t size;
public:
    bytebit(const size_t& size);
    bool operator==(const bytebit& bb) const;
    bytebit& operator<<(const char& c);
    bytebit& operator<<(const std::string& str);
    const std::string& get_data() const override;
    const size_t& get_size() const;
    void add_null();
    void add_one();
    void pop_back();
    void clear();
};

class sstrtobb : public ssbb_base {
protected:
    void write(ssbb_base* ptr_ssbb);
public:
    sstrtobb();
    sstrtobb(const std::string& str);
    sstrtobb& operator>>(bytebit& bb);
    sstrtobb& operator<<(bytebit bb);
    sstrtobb& operator<<(sstrtobb ptr_ssbb);
    sstrtobb& operator<<(const std::string& str);
    sstrtobb& operator>>(std::string& str);
    sstrtobb& operator>>(uint8_t& c);
    sstrtobb& operator<<(const uint8_t& c);
    const std::string& get_data() const override;
    void try_write(std::ostream& os);
};
