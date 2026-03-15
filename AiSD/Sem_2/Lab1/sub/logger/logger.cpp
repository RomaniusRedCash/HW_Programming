#include "logger.h"

logger::logger(uint8_t lvl) : lvl(lvl) {

}

logger& logger::operator<<(std::ostream& (*manip)(std::ostream&)) {
    if (lvl & log_ns::CONS_LVL & logger_demon::lvl)
        std::cout<<std::endl;
    if (lvl & log_ns::FILE_LVL & logger_demon::lvl)
        logger_demon::logfile<<std::endl;

    if (lvl & log_ns::DEV_ONLY & logger_demon::lvl) {
        std::cout<<std::endl;
        logger_demon::logfile<<std::endl;
    }

    return *this;
}

logger& logger::write(const char* s, std::streamsize n) {
    if (lvl & log_ns::CONS_LVL & logger_demon::lvl)
        std::cout.write(s, n);
    // if (lvl & log_ns::FILE_LVL & logger_demon::lvl)
    logger_demon::logfile.write(s, n);

    if (lvl & log_ns::DEV_ONLY & logger_demon::lvl) {
        std::cout.write(s, n);
        logger_demon::logfile.write(s, n);
    }

    return *this;
}

void logger_demon::set_log_lvl(const uint8_t& lvl) {
    log_dem.lvl = lvl;
}

void logger_demon::add_log_lvl(const uint8_t& lvl) {
    log_dem.lvl |= lvl;
}

void logger_demon::open_logfile(const char* name) {
    logfile.close();
    logfile.open(name);
}

logger_demon::logger_demon() {
}

logger_demon::~logger_demon() {
    logfile.close();
}
