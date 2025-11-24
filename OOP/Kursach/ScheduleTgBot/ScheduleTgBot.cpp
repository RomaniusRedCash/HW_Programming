#include "ScheduleTgBot.h"

void ScheduleTgBot::makeCommandFromMes(){
	static const std::unordered_map<std::string, char> tempMap = {
	{"near_lesson", command::near_lesson}, {"tommorow", command::tommorow},
	{"all", command::all}, {"day", command::day}
	};
	json::value commandJson = propBot.at("commands");
	for (const std::pair<std::string, char>& i : tempMap) {
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
		
		default:
			break;
		}
	}


	if (mesText.find(command::REG) == 0 && mesText.size() >= 4 + strlen(command::REG)) {
		int result;
		std::from_chars_result res = std::from_chars(mesText.data() + strlen(command::REG), mesText.data() + mesText.size(), result);
		if (res.ec != std::errc::invalid_argument) {
			usersGroup[idUser] = result;
			tgClient->sendMessageTextOnly(idUser, "Группа установлена.");
			tgClient->sendMessage(idUser, propBot.at("messages").at("success"));
			return;
		}
		tgClient->sendMessageTextOnly(idUser, "Повторите попытку.");
		return;
	}
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

ScheduleTgBot::ScheduleTgBot() {
	std::fstream file(PROP, std::ios::in);
	if (!file.is_open()) throw "Файл не открыт!";
	std::stringstream ss;
	ss << file.rdbuf();
	propBot = json::parse(ss);
	file.close();

	makeCommandFromMes();

	tgClient = new TgClient(getEnv("SCHED_BOT_TOKEN"), propBot);
	file.open(SAVE_USERS, std::ios::in | std::ios::binary);
	if (file.is_open()) {
		int64_t idUser = 0;
		while (file.read(reinterpret_cast<char*>(idUser), sizeof(idUser)))
			file.read(reinterpret_cast<char*>(usersGroup[idUser]), sizeof(usersGroup[idUser]));
	}
}

void ScheduleTgBot::upDate() {
	json::array mesArr = tgClient->getUpdates();
	if (mesArr.empty()) return;
	for (const json::value& i : mesArr)
		processMes(i.at("message"));
}

ScheduleTgBot::~ScheduleTgBot() {
	delete tgClient;
	std::fstream fileSave(SAVE_USERS, std::ios::out | std::ios::binary);
	if (fileSave.is_open())
		for (const std::pair<int64_t, short>& i : usersGroup)
			fileSave << i.first << i.second;
	//else loger("error save users data");
}

#undef PROP 
#undef SAVE_USERS 
