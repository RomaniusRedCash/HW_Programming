#include "ScheduleTgBot.h"

void ScheduleTgBot::processMes(const json::value& jsonMes) {
	std::string mesText = jsonMes.at_as_str("text");
	int64_t idUser = jsonMes.at("from").at_as_int("id");
	if (mesText.find(command::REG) == 0 && mesText.size() >= 4 + strlen(command::REG)) {
		int result;
		std::from_chars_result res = std::from_chars(mesText.data() + strlen(command::REG), mesText.data() + mesText.size(), result);
		if (res.ec != std::errc::invalid_argument) {
			usersGroup[idUser] = result;
			tgClient->sendMessageTextOnly(idUser, "Группа установлена.");
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
#elif
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