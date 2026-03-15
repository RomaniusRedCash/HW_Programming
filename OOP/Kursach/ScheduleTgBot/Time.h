#pragma once

#include "Header.h"

class Time {
	inline static const std::vector<std::string> daysArr = { "MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN" };

public:
	static int getNowDay();
	static int getNumByDay(const std::string& day);
	static const std::string& getDayByNum(const int& day);
	static std::chrono::seconds getNowSeconds();
};