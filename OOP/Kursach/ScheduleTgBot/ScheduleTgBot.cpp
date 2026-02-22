#include "ScheduleTgBot.h"

void ScheduleTgBot::makeCommandFromMes(){
	static const std::unordered_map<std::string, command_bot::eCommand> tempMap = {
	{"near_lesson", command_bot::near_lesson}, {"tommorow", command_bot::tommorow},
	{"all", command_bot::all},{"all1", command_bot::all1},{"all2", command_bot::all2}, {"day_of_week", command_bot::day_of_week}, {"/start", command_bot::start}
	};
	json::value commandJson = propBot.at("commands");
	for (const std::pair<const std::string, command_bot::eCommand>& i : tempMap) {
		commandMap[commandJson.at_as_str(i.first)] = i.second;
	}
}


void ScheduleTgBot::processMes(const json::value& jsonMes) {
	std::string mesText = jsonMes.at_as_str("text");
	int64_t idUser = jsonMes.at("from").at_as_int("id");
	const uint16_t& group = usersGroup[idUser];
	json::value lessonJson;

	if(commandMap.count(mesText) != 0){
		switch (commandMap[mesText]) {
		case command_bot::near_lesson:
			lessonJson = etuClient->getNearLesson(group);
			if (lessonJson.is_object() && lessonJson.get_object().empty()) break;
			tgClient->sendMessageTextOnly(idUser, mesMakeForOneLesson(lessonJson));


			return;
			break;
		case command_bot::tommorow:

			lessonJson = etuClient->getTommorow(group);
			if (lessonJson.is_object() && lessonJson.get_object().empty()) break;
			tgClient->sendMessageTextOnly(idUser, mesMakeForDay(lessonJson));

			return;
			break;
		case command_bot::all:
			lessonJson = etuClient->getAll(group);
			if (lessonJson.is_object() && lessonJson.get_object().empty()) break;
			tgClient->sendMessageTextOnly(idUser, mesMakeForAll(lessonJson));
			return;
			break;
		case command_bot::all1:
			lessonJson = etuClient->getAll(group);
			if (lessonJson.is_object() && lessonJson.get_object().empty()) break;
			tgClient->sendMessageTextOnly(idUser, mesMakeForAll(lessonJson, "1"));
			return;
			break;
		case command_bot::all2:
			lessonJson = etuClient->getAll(group);
			if (lessonJson.is_object() && lessonJson.get_object().empty()) break;
			tgClient->sendMessageTextOnly(idUser, mesMakeForAll(lessonJson, "2"));
			return;
			break;
		case command_bot::day_of_week:
			tgClient->sendMessage(idUser, propBot.at("messages").at("day_of_week"));
			return;
			break;
		case command_bot::start:
			tgClient->sendMessage(idUser, propBot.at("messages").at(mesText));
			return;
			break;
		default:
			break;
		}
		return tgClient->sendMessageTextOnly(idUser, "Неверные данные группы или команда.");
	}


	if (mesText.find(command_bot::REG) == 0 && mesText.size() >= 4 + strlen(command_bot::REG)) {
		int result = 0;
		std::from_chars_result res = std::from_chars(mesText.data() + strlen(command_bot::REG), mesText.data() + mesText.size(), result);
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
	const uint16_t& group = usersGroup[idUser];
	std::string command = jsonMes.at_as_str("data");
	if (command.find(command_bot::DAY_OF_WEEK) != std::string::npos){
		json::value jv = propBot.at("messages").at("day");
		for (json::value& i : jv.at("reply_markup").at("inline_keyboard").as_array())
			for (json::value& j : i.as_array())
				j.as_object()["callback_data"] = std::string() + j.at_as_str("callback_data") + command.substr(std::strlen(command_bot::DAY_OF_WEEK),1);
		return tgClient->sendMessage(idUser, jv);
	}
	json::value lessons = etuClient->getDay(group, command.substr(0,3));
	if (lessons.is_object() && lessons.get_object().empty()) return tgClient->sendMessageTextOnly(idUser, "Неверные данные группы.");
	std::string week = command.substr(3,1);
	tgClient->sendMessageTextOnly(idUser, mesMakeForDay(lessons, week));
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

std::string ScheduleTgBot::mesMakeForAll(const json::value& lessonsJson, const std::string& week) {
	std::stringstream ss;
	for (int i = 0; i < 7; i++) {
		ss << mesMakeForDay(lessonsJson.at(std::to_string(i)), week);
	}
	return ss.str();
}

std::string ScheduleTgBot::mesMakeForDay(const json::value& lessonsJson, const std::string& week) {
	std::stringstream ss;

	ss << "<b><u>" << lessonsJson.at_as_str("name") << "</u></b>\n";
	json::array lessonsJsonArray = lessonsJson.at("lessons").as_array();
	for (const json::value& i : lessonsJsonArray){
		std::string weekT = i.at_as_str("week");
		if (weekT == "3" || weekT == week || week == "3")
			ss << mesMakeForOneLesson(i);
	}
	ss << "\n\n";
	return ss.str();
}

std::string ScheduleTgBot::mesMakeForOneLesson(const json::value& lessonJson) {
	std::stringstream ss;
	ss << "   <b>" << lessonJson.at_as_str("name") << "</b>\n";
	ss << "      " << lessonJson.at_as_str("subjectType");
	if (std::string(lessonJson.at_as_str("week")) != "3")
		ss << "   " << "<i>неделя " << lessonJson.at_as_str("week") << "</i>";
	ss << '\n';
	if (strlen(lessonJson.at_as_str("teacher")) > 0)
		ss << "      " << lessonJson.at_as_str("teacher") << '\n';
	if (strlen(lessonJson.at_as_str("second_teacher")) > 0)
		ss << "      " << lessonJson.at_as_str("second_teacher") << '\n';
	ss << "       " << lessonJson.at_as_str("start_time") << '-' << lessonJson.at_as_str("end_time") << "   ";
	if (strlen(lessonJson.at_as_str("room")) > 0)
		ss << "       " << lessonJson.at_as_str("room") << " - ";
	if (strlen(lessonJson.at_as_str("form")) > 0)
		ss << lessonJson.at_as_str("form") << '\n';
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
}

#undef PROP 
#undef SAVE_USERS 