#include "ScheduleTgBot.h"


int main() {
	ScheduleTgBot bot;
	std::string command = "null";
	std::thread cinThr([&command] { while (command != "stop") std::cin >> command; });
	while (command != "stop") {
		bot.upDate();
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
	cinThr.join();
	return 0;
}
