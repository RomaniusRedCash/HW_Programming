#pragma once

#include "Header.h"
#include "TgClient.h"

#define PROP "BOTproperties.json"
#define SAVE_USERS "USERS"

namespace command {
	const char REG[] = "/reg ";
}

class ScheduleTgBot {
	TgClient* tgClient;
	json::value propBot;

	std::unordered_map<int64_t, uint16_t> usersGroup;

	void processMes(const json::value& jsonMes);
	std::string getEnv(const char* nameEnv);
public:
	ScheduleTgBot();
	void upDate();
	virtual ~ScheduleTgBot();
};

