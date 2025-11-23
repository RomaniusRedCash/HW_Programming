#pragma once

#include "Header.h"

class ClientBase {
protected:
	std::string url;
	inline static const std::string clientName = "ScheduleTgBot 13";
	inline static net::io_context ioc;

	beast::error_code ec;
	//net::io_context ctx;
	net::ip::tcp::resolver resolver;
	beast::ssl_stream<beast::tcp_stream>* sslStream = nullptr;
	const json::value& prop;

	std::chrono::milliseconds maxCooldown = std::chrono::milliseconds(20000);

	bool connectServer();
	void disconnectServer();
	virtual json::value someRequest(const http::verb& method, const json::value& jsonRequest);
public:
	ClientBase(const const char* url, const json::value& prop);
	virtual ~ClientBase();
};
