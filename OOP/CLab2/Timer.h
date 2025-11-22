#pragma once

#include <chrono>
#include <iostream>

class Timer {
	std::chrono::high_resolution_clock::time_point startTime;
	std::ostream& s;
public:
	Timer(std::ostream& s);
	~Timer();
};

