#pragma once

#include "Header.h"
#include "TgClient.h"

#define PROP "BOTproperties.json"
#define SAVE_USERS "USERS"

namespace command {
	const char REG[] = "/reg ";
	enum eCommand : char {
		near_lesson,
		tommorow,
		day,
		all
	};
	enum eDay : char{
		MON, TUE, WED, THU, FRI, SAT, SUN
	};
}

class ScheduleTgBot {
	TgClient* tgClient;
	json::value propBot;

	std::unordered_map<int64_t, uint16_t> usersGroup;
	inline static std::unordered_map<std::string, char> commandMap;

	void makeCommandFromMes();
	void processMes(const json::value& jsonMes);
	std::string getEnv(const char* nameEnv);
public:
	ScheduleTgBot();
	void upDate();
	virtual ~ScheduleTgBot();
};

