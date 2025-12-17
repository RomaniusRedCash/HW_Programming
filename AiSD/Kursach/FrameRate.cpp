#include "FrameRate.h"

FrameRate::FrameRate() {
	old = std::chrono::system_clock::now();
}

void FrameRate::wait() {
	while (!canDo());
}

bool FrameRate::canDo() {
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	if (std::chrono::duration_cast<std::chrono::microseconds>(now - old) * fps >= std::chrono::seconds(1)) {
		old = now;
		return true;
	}
	return false;
}

void FrameRate::newRate() {
	for (std::function<void(void)>& f : vTriger)
		f();
	frame++;
}

void FrameRate::addTriger(std::function<void(void)> f) {
	vTriger.push_back(f);
}
