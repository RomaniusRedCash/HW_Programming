#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <cinttypes>

namespace log_ns {
    enum LOG_LVL : uint8_t{
        NON_LVL = 0,
        FILE_LVL = 1,
        CONS_LVL = 1 << 1,

        DEV_ONLY = 1 << 2,

        NORMAL_LVL = 1 << 3,
        HARD_LVL = 1 << 4,

        // DEFAULT_LVL = FILE_LVL | CONS_LVL,
    };
}



class logger_demon;
class logger {
    uint8_t lvl = log_ns::NON_LVL;
    //
    friend logger_demon;
    bool check_flag();
public:
    logger() {};
    logger(uint8_t lvl);
    template<typename T>
    logger& operator<<(const T& t);
    logger& operator<<(std::ostream& (*manip)(std::ostream&));
    logger& write(const char* s, std::streamsize n);
    static void flush();
};

class logger_demon {
    static logger_demon log_dem;

    logger_demon();
    logger_demon(const logger_demon&) = delete;
    logger_demon& operator=(const logger_demon&) = delete;
    // friend logger;

public:
    inline static std::ofstream logfile;
    inline static uint8_t lvl = log_ns::NON_LVL;
    static void set_log_lvl(const uint8_t& lvl);
    static void add_log_lvl(const uint8_t& lvl);
    static void open_logfile(const char* name);

    ~logger_demon();
};

template<typename T>
logger& logger::operator<<(const T& t) {
    if (!check_flag()) return *this;
    if (log_ns::CONS_LVL & logger_demon::lvl)
        std::cout<<t;
    if (log_ns::FILE_LVL & logger_demon::lvl)
        logger_demon::logfile<<t;
    logger_demon::logfile.flush();

    return *this;
}
