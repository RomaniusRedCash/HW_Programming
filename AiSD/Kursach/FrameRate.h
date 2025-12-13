#pragma once

#include "Header.h"

extern const uint8_t fps;

class FrameRate {
	std::chrono::system_clock::time_point old;
	static std::vector<std::function<void(void)>> vTriger;
public:
	FrameRate();
	void wait();
	bool canDo();
	static void newRate();
	static void addTriger(std::function<void(void)> f);
	static size_t frame;
};

