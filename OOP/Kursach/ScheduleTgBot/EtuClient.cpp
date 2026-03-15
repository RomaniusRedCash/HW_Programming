#include "EtuClient.h"

json::value EtuClient::getNearLesson(const uint16_t& idGroup) {
	int day = Time::getNowDay();
	long long nowSec = Time::getNowSeconds().count();
	json::value nowLesson = getDay(idGroup, Time::getDayByNum(day));
	if (nowLesson.as_object().empty()) return nowLesson;
	json::array nowLessonArr = nowLesson.at("lessons").as_array();
	for (const json::value& i : nowLessonArr)
		if (i.at_as_int("end_time_seconds") > nowSec)
			return i;
	nowLesson = getTommorow(idGroup);
	if (nowLesson.as_object().empty()) return nowLesson;
	return nowLesson.at("lessons").as_array().front();
}

json::value EtuClient::getTommorow(const uint16_t& idGroup) {
	json::value tommorowLesson;
	int nowDay = Time::getNowDay() + 1;
	char days= 0;
	while ((tommorowLesson.is_null() || tommorowLesson.as_object().empty()) && days < 7) {
		tommorowLesson = getDay(idGroup, Time::getDayByNum((nowDay + days) % 7));
		days++;
	}
	return tommorowLesson;
}

json::value EtuClient::getAll(const uint16_t& idGroup) {
	json::value jsonAnswer = someRequest(http::verb::get, prop.at("etu_request"), idGroup);
	return jsonAnswer;
}

json::value EtuClient::getDay(const uint16_t& idGroup, const std::string& day) {
	int nowDayNum = Time::getNumByDay(day);
	json::value sendJson = prop.at("etu_request");
	sendJson.as_object()["method"] = std::string(sendJson.at_as_str("method"))+"&weekDay=" + day;
	json::value jsonAnswer = someRequest(http::verb::get, sendJson, idGroup);
	if (jsonAnswer.get_object().empty()) return jsonAnswer;
	return jsonAnswer.at(std::to_string(nowDayNum));
}

json::value EtuClient::someRequest(const http::verb& method, json::value jsonRequest, const uint16_t& idGroup) {
	jsonRequest.as_object()["method"] = std::string(jsonRequest.at_as_str("method")) + "&groupNumber=" + std::to_string(idGroup);
	json::value jsonAnswer = someRequest(method, jsonRequest);
	if (jsonAnswer.get_object().empty()) return jsonAnswer;
	return jsonAnswer.at(std::to_string(idGroup)).at("days");
}

EtuClient::EtuClient(const json::value& prop) : ClientBase(URL_SERVER, prop) {

}

EtuClient::~EtuClient()
{
}

#undef URL_SERVER