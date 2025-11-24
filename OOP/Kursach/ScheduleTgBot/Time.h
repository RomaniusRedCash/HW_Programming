#pragma once

#include "Header.h"

static class Time {
	inline static const std::vector<std::string> daysArr = { "MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN" };

public:
	static const int getNowDay();
	static const int getNumByDay(const std::string& day);
	static const std::string& getDayByNum(const int& day);
	static std::chrono::seconds getNowSeconds();
};

