#pragma once

#include "Header.h"

extern const uint8_t fps;

class FrameRate {
	std::chrono::system_clock::time_point old;
	inline static std::vector<std::function<void(void)>> vTriger;
public:
	inline static size_t frame = 0;

	FrameRate();
	void wait();
	bool canDo();
	static void newRate();
	static void addTriger(std::function<void(void)> f);
};

