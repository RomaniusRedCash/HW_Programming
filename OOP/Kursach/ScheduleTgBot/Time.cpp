#include "Time.h"

const int Time::getNowDay() {
    std::chrono::time_point nowDay = floor<std::chrono::days>(std::chrono::system_clock::now());

    std::chrono::weekday wd(nowDay);
    return wd.c_encoding();
}

const int Time::getNumByDay(const std::string& day) {
    return std::find(daysArr.begin(), daysArr.end(), day) - daysArr.begin();
}

const std::string& Time::getDayByNum(const int& day) {
    return daysArr[day];
}

std::chrono::seconds Time::getNowSeconds() {
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point nowDay = floor<std::chrono::days>(now);

    return duration_cast<std::chrono::seconds>(now - nowDay);;
}
