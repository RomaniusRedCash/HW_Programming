#include "ScheduleTgBot.h"


int main() {
	ScheduleTgBot bot;
	while (true) {
		bot.upDate();
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
	return 0;
}
