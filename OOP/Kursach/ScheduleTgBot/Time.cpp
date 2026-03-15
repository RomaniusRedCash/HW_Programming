#include "Time.h"

int Time::getNowDay() {
    std::chrono::time_point nowDay = floor<std::chrono::days>(std::chrono::system_clock::now());

    std::chrono::weekday wd(nowDay);
    return wd.iso_encoding() - 1;
}

int Time::getNumByDay(const std::string& day) {
    return std::find(daysArr.begin(), daysArr.end(), day) - daysArr.begin();
}

const std::string& Time::getDayByNum(const int& day) {
    return daysArr[day];
}

std::chrono::seconds Time::getNowSeconds() {
    std::chrono::local_time now = std::chrono::zoned_time(std::chrono::current_zone(), std::chrono::system_clock::now()).get_local_time();
    std::chrono::local_time nowDay = std::chrono::floor<std::chrono::days>(now);

    return duration_cast<std::chrono::seconds>(now - nowDay);
}