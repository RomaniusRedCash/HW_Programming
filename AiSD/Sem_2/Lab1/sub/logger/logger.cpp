#include "logger.h"

using namespace log_ns;

bool logger::check_flag() {
    return (
        (
            !(lvl & (NORMAL_LVL | HARD_LVL)) ||
            (lvl & (NORMAL_LVL | HARD_LVL) & logger_demon::lvl)
        ) && (
            !(lvl & DEV_ONLY) ||
            (lvl & DEV_ONLY & logger_demon::lvl)
        )

    );
}

logger::logger(uint8_t lvl) : lvl(lvl) {

}

logger& logger::operator<<(std::ostream& (*manip)(std::ostream&)) {
    if (!check_flag()) return *this;
    if (CONS_LVL & logger_demon::lvl)
        std::cout<<std::endl;
    if (FILE_LVL & logger_demon::lvl)
        logger_demon::logfile<<std::endl;
    return *this;
}

logger& logger::write(const char* s, std::streamsize n) {
    if (!check_flag()) return *this;
    if (CONS_LVL & logger_demon::lvl)
        std::cout.write(s, n);
    logger_demon::logfile.write(s, n);
    return *this;
}

void logger::flush() {
    logger_demon::logfile.flush();
    std::cout.flush();
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
