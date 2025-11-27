#include "ScheduleTgBot.h"

void ScheduleTgBot::makeCommandFromMes(){
	static const std::unordered_map<std::string, char> tempMap = {
	{"near_lesson", command::near_lesson}, {"tommorow", command::tommorow},
	{"all", command::all}, {"day", command::day}, {"/start", command::start}
	};
	json::value commandJson = propBot.at("commands");
	for (const std::pair<const std::string, char>& i : tempMap) {
		commandMap[commandJson.at_as_str(i.first)] = i.second;
	}
}


void ScheduleTgBot::processMes(const json::value& jsonMes) {
	std::string mesText = jsonMes.at_as_str("text");
	int64_t idUser = jsonMes.at("from").at_as_int("id");

	if(commandMap.count(mesText) != 0){
		switch (commandMap[mesText]) {
		case command::near_lesson:
			
			return;
			break;
		case command::tommorow:
			
			return;
			break;
		case command::all:
			
			return;
			break;
		case command::day:
			tgClient->sendMessage(idUser, propBot.at("messages").at("day"));
			return;
			break;
		case command::start:
			tgClient->sendMessage(idUser, propBot.at("messages").at(mesText));
			return;
			break;
		default:
			break;
		}
	}


	if (mesText.find(command::REG) == 0 && mesText.size() >= 4 + strlen(command::REG)) {
		int result = 0;
		std::from_chars_result res = std::from_chars(mesText.data() + strlen(command::REG), mesText.data() + mesText.size(), result);
		if (res.ec != std::errc::invalid_argument) {
			usersGroup[idUser] = result;
			tgClient->sendMessage(idUser, propBot.at("messages").at("success"));
			return;
		}
		tgClient->sendMessageTextOnly(idUser, "Повторите попытку.");
		return;
	}
}

void ScheduleTgBot::processCallBack(const json::value& jsonMes) {
	int64_t idUser = jsonMes.at("from").at_as_int("id");
	const short& group = usersGroup[idUser];
	std::string day = jsonMes.at_as_str("data");
	json::value lessons = etuClient->getDay(group, day);
	if (lessons.is_object() && lessons.get_object().empty()) return tgClient->sendMessageTextOnly(idUser, "Неверные данные группы.");
	tgClient->sendMessageTextOnly(idUser, mesMakeForDay(lessons));
}

std::string ScheduleTgBot::getEnv(const char* nameEnv) {
	std::string env;
#ifdef _WIN32
	char* pValue;
	size_t len;
	_dupenv_s(&pValue, &len, nameEnv);
	env = pValue;
	free(pValue);
#else
	env = getenv(nameEnv);
#endif
	return env;
}

std::string ScheduleTgBot::mesMakeForDay(const json::value& lessonsJson) {
	std::stringstream ss;
	ss << "<b>" << lessonsJson.at_as_str("name") << "</b>\n";
	json::array lessonsJsonArray = lessonsJson.at("lessons").as_array();
	for (const json::value& i : lessonsJsonArray)
		ss << mesMakeForOneLesson(i) << "\n\n";
	return ss.str();
}

std::string ScheduleTgBot::mesMakeForOneLesson(const json::value& lessonJson) {
	std::stringstream ss;
	ss << "\t\t" << lessonJson.at_as_str("name") << '\n';
	if (strlen(lessonJson.at_as_str("teacher")) > 0)
		ss << "\t\t\t\t" << lessonJson.at_as_str("teacher") << '\n';
	if (strlen(lessonJson.at_as_str("second_teacher")) > 0)
		ss << "\t\t\t\t" << lessonJson.at_as_str("second_teacher") << '\n';
	ss << "\t\t\t\t" << lessonJson.at_as_str("start_time") << '-' << lessonJson.at_as_str("end_time") << "\t\t\t\t";
	if (strlen(lessonJson.at_as_str("room")) > 0)
		ss << lessonJson.at_as_str("room") << " -";
	if (strlen(lessonJson.at_as_str("form")) > 0)
		ss << ' ' << lessonJson.at_as_str("form") << '\n';
	return ss.str();
}

ScheduleTgBot::ScheduleTgBot() {
	std::fstream file(PROP, std::ios::in);
	if (!file.is_open()) throw "Файл не открыт!";
	std::stringstream ss;
	ss << file.rdbuf();
	propBot = json::parse(ss);
	file.close();

	makeCommandFromMes();

	tgClient = new TgClient(getEnv("SCHED_BOT_TOKEN"), propBot);
	etuClient = new EtuClient(propBot);
	file.open(SAVE_USERS, std::ios::in | std::ios::binary);
	if (file.is_open()) {
		int64_t idUser = 0;
		size_t sizeMap = 0;
		usersGroup.reserve(sizeMap);
		file.read(reinterpret_cast<char*>(&sizeMap), sizeof(sizeMap));
		for (size_t i = 0; i < sizeMap; i++) {
			file.read(reinterpret_cast<char*>(&idUser), sizeof(idUser));
			file.read(reinterpret_cast<char*>(&usersGroup[idUser]), sizeof(usersGroup[idUser]));
		}
	}
}

void ScheduleTgBot::upDate() {
	json::array mesArr = tgClient->getUpdates();
	if (mesArr.empty()) return;
	for (const json::value& i : mesArr)
		if (i.as_object().contains("message")) 
			processMes(i.at("message"));
		else if (i.as_object().contains("callback_query"))
			processCallBack(i.at("callback_query"));
}

ScheduleTgBot::~ScheduleTgBot() {
	delete tgClient;
	delete etuClient;
	std::fstream fileSave(SAVE_USERS, std::ios::out | std::ios::binary);
	if (fileSave.is_open()) {
		size_t size = usersGroup.size();
		fileSave.write(reinterpret_cast<const char*>(&size), sizeof(size));
		for (const std::pair<const int64_t, uint16_t>& i : usersGroup) {
			fileSave.write(reinterpret_cast<const char*>(&i.first), sizeof(i.first));
			fileSave.write(reinterpret_cast<const char*>(&i.second), sizeof(i.second));
		}
		fileSave.close();
	}

	//else loger("error save users data");
}

#undef PROP 
#undef SAVE_USERS 
