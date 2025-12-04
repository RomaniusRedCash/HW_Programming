#pragma once

#include "ClientBase.h"
#include "Time.h"

#define URL_SERVER "digital.etu.ru"

class EtuClient : public ClientBase {
public:
	ClientBase::someRequest;
	json::value getNearLesson(const uint16_t& idGroup);
	json::value getTommorow(const uint16_t& idGroup);
	json::value getAll(const uint16_t& idGroup);
	json::value getDay(const uint16_t& idGroup, const std::string& day);
	json::value someRequest(const http::verb& method, json::value jsonRequest, const uint16_t& idGroup);
	EtuClient(const json::value& prop);
	~EtuClient() override;
};

