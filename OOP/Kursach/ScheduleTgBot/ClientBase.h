#pragma once

#include "Header.h"

class ClientBase {
protected:
	std::string url;
	inline static const std::string clientName = "ScheduleTgBot 13";
	std::chrono::milliseconds maxCooldown = std::chrono::milliseconds(20000);
	const json::value& prop;
	inline static net::io_context ioc;
	beast::ssl_stream<beast::tcp_stream>* sslStream = nullptr;
	net::ip::tcp::resolver resolver;
	beast::error_code ec;

	bool connectServer();
	void disconnectServer();
	virtual json::value someRequest(const http::verb& method, const json::value& jsonRequest);
public:
	ClientBase(const char* url, const json::value& prop);
	virtual ~ClientBase();
};