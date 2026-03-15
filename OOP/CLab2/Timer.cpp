#include "Timer.h"

Timer::Timer(std::ostream& s) : s(s){
	startTime = std::chrono::high_resolution_clock::now();
}

Timer::~Timer() {
	s << "Прошло " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - startTime).count() << "ms" << std::endl;
}
