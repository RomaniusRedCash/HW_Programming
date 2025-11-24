#pragma once

#include "Header.h"
#include "TgClient.h"
#include "EtuClient.h"

#define PROP "BOTproperties.json"
#define SAVE_USERS "USERS.bin"

namespace command {
	const char REG[] = "/reg ";
	enum eCommand : char {
		near_lesson,
		tommorow,
		day,
		all,
		start
	};
	//enum eDay : char{
	//	MON, TUE, WED, THU, FRI, SAT, SUN
	//};
}

class ScheduleTgBot {
	TgClient* tgClient;
	EtuClient* etuClient;
	json::value propBot;

	std::unordered_map<int64_t, uint16_t> usersGroup;
	inline static std::unordered_map<std::string, char> commandMap;

	void makeCommandFromMes();
	void processMes(const json::value& jsonMes);
	void processCallBack(const json::value& jsonMes);
	std::string getEnv(const char* nameEnv);
	std::string mesMakeForDay(const json::value& lessonsJsonArray);
	std::string mesMakeForOneLesson(const json::value& lessonJson);
public:
	ScheduleTgBot();
	void upDate();
	virtual ~ScheduleTgBot();
};

