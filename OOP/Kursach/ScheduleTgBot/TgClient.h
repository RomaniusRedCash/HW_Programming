#pragma once

//#include "boost/property_tree/ptree.hpp"
//#include "boost/property_tree/json_parser.hpp"

#include "Header.h"
#include "ClientBase.h"

#define LAST_MES_FILE "last_mes.bin"
#define URL_SERVER "api.telegram.org"

//#define GET_CHAT_ID(jsonValue) jsonValue.at("chat").at("id").as_int64()

//namespace ppt = boost::property_tree;

class TgClient : public ClientBase {
	std::string tgToken;

	size_t lastMes = 0;

public:
	TgClient(const std::string& token, const json::value& prop);
	json::array getUpdates();
	json::value someRequest(const http::verb& method, const json::value& jsonValue) override;
	void sendMessage(const int64_t userId, json::value& data);
	void sendMessageTextOnly(const int64_t userId, const std::string& data);
	~TgClient() override;
};
