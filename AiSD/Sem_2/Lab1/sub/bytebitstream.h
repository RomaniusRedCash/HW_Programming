#pragma once

#include <iostream>
#include <string>
#include <cinttypes>
#include "../sub/logger/logger.h"
#include <bitset>

class ssbb_base {
protected:
    std::string data;
    int8_t buffer_sdvig_size = 0;

    void sdvig_l(const uint8_t& size);
    void sdvig_r(const uint8_t& size);
public:
    ssbb_base() = default;
    ssbb_base(const std::string& str);
    virtual const std::string& get_data() const = 0;
};

class sstrtobb;

class bytebit : public ssbb_base{
protected:
    // std::string data;
    friend sstrtobb;

    size_t size;
public:
    bytebit(const size_t& size);
    bytebit& operator<<(const char& c);
    const std::string& get_data() const override;
    const size_t& get_size() const;
};

class sstrtobb : public ssbb_base {
public:
    sstrtobb();
    sstrtobb(const std::string& str);
    sstrtobb& operator>>(bytebit& bb);
    sstrtobb& operator<<(bytebit bb);
    const std::string& get_data() const override;
    void try_write(std::ostream& os);
};

// class sbbtostr : protected ssbb_base {
// public:
//     // sbbtostr(const std::string& str);
//
// };
