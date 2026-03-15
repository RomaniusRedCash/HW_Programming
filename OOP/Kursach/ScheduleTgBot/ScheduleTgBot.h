#pragma once

#include "Header.h"
#include "TgClient.h"
#include "EtuClient.h"

#define PROP "BOTproperties.json"
#define SAVE_USERS "USERS.bin"

namespace command_bot {
	const char REG[] = "/reg ";
	const char DAY_OF_WEEK[] = "WEEK";
	enum eCommand : uint8_t {
		near_lesson,
		tommorow,
		day_of_week,
		all,
		all1,
		all2,
		start
	};
}

class ScheduleTgBot {
	TgClient* tgClient;
	EtuClient* etuClient;
	json::value propBot;
	std::unordered_map<int64_t, uint16_t> usersGroup;
	inline static std::unordered_map<std::string, command_bot::eCommand> commandMap;

	void makeCommandFromMes();
	void processMes(const json::value& jsonMes);
	void processCallBack(const json::value& jsonMes);
	std::string getEnv(const char* nameEnv);
	std::string mesMakeForAll(const json::value& lessonsJson, const std::string& week = "3");
	std::string mesMakeForDay(const json::value& lessonsJson, const std::string& week = "3");
	std::string mesMakeForOneLesson(const json::value& lessonJson);
public:
	ScheduleTgBot();
	void upDate();
	virtual ~ScheduleTgBot();
};