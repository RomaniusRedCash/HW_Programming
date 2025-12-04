#include "EtuClient.h"

//json::value EtuClient::someRequest(const http::verb& method, const json::value& jsonValue) {
//	return json::value();
//}

json::value EtuClient::getNearLesson(const uint16_t& idGroup) {
	int day = Time::getNowDay();
	long long nowSec = Time::getNowSeconds().count();
	//json::array nowLesson = getDay(idGroup, Time::getDayByNum(day)).at(std::to_string(day)).at("lessons").as_array();
	json::value nowLesson = getDay(idGroup, Time::getDayByNum(day));
	if (nowLesson.as_object().empty()) return nowLesson;
	json::array nowLessonArr = nowLesson.at("lessons").as_array();
	for (const json::value& i : nowLessonArr)
		if (i.at_as_int("start_time_seconds") > nowSec && i.at_as_int("end_time_seconds") > nowSec)
			return i;
	return json::value();
}

json::value EtuClient::getTommorow(const uint16_t& idGroup) {
	json::value tommorowLesson;
	int nowDay = Time::getNowDay() + 1;
	char days= 0;
	while (tommorowLesson.as_object().empty() && days < 7) {
		tommorowLesson = getDay(idGroup, Time::getDayByNum((nowDay + days) % 7)).at("lessons");
		days++;
	}
	return tommorowLesson;
}

json::value EtuClient::getDay(const uint16_t& idGroup, const std::string& day) {
	int nowDayNum = Time::getNumByDay(day);
	json::value sendJson = prop.at("etu_request");
	std::string methodStr = sendJson.at_as_str("method");
	sendJson.as_object()["method"] = methodStr + "&groupNumber=" + std::to_string(idGroup) + "&weekDay=" + day;
	json::value jsonAnswer = someRequest(http::verb::get, sendJson, idGroup);
	if (jsonAnswer.get_object().empty()) return jsonAnswer;
	return jsonAnswer.at(std::to_string(nowDayNum));
}

json::value EtuClient::someRequest(const http::verb& method, const json::value& jsonRequest, const uint16_t& idGroup) {
	json::value jsonAnswer = ClientBase::someRequest(method, jsonRequest);
	if (jsonAnswer.get_object().empty()) return jsonAnswer;
	return jsonAnswer.at(std::to_string(idGroup)).at("days");
}

EtuClient::EtuClient(const json::value& prop) : ClientBase(URL_SERVER, prop) {

}

EtuClient::~EtuClient()
{
}

#undef URL_SERVER
