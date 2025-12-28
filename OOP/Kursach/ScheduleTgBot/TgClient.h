#pragma once

#include "Header.h"
#include "ClientBase.h"

#define LAST_MES_FILE "last_mes.bin"
#define URL_SERVER "api.telegram.org"

class TgClient : public ClientBase {
	std::string tgToken;
	size_t lastMes = 0;

	json::value someRequest(const http::verb& method, const json::value& jsonValue) override;
public:
	TgClient(const std::string& token, const json::value& prop);
	json::array getUpdates();
	void sendMessage(const int64_t userId, json::value& data);
	void sendMessageTextOnly(const int64_t userId, const std::string& data);
	~TgClient() override;
};