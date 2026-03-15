#include "TgClient.h"

json::value TgClient::someRequest(const http::verb& method, const json::value& jsonValue) {
    json::value sendJsonValue = jsonValue;
    sendJsonValue.as_object()["method"] = "/bot" + tgToken + '/' + jsonValue.at_as_str("method");
    return ClientBase::someRequest(method, sendJsonValue);
}

json::array TgClient::getUpdates() {
    json::array mesArr;
    json::value Req = prop.at("getUpdates");
    Req.as_object()["offset"] = lastMes + 1;
    mesArr = someRequest(http::verb::get, Req).at("result").as_array();
    if (!mesArr.empty())
        lastMes = mesArr.back().at_as_int("update_id");
    return mesArr;
}

void TgClient::sendMessage(const int64_t userId, json::value& data) {
    data.as_object()["chat_id"] = userId;
    someRequest(http::verb::post, data);
}

void TgClient::sendMessageTextOnly(const int64_t userId, const std::string& data) {
    json::value sendJson = prop.at("messages").at("text_only");
    sendJson.as_object()["text"] = data;
    sendMessage(userId, sendJson);
}

TgClient::TgClient(const std::string& token, const json::value& prop) : ClientBase(URL_SERVER, prop) {
    tgToken = token;
    std::fstream fileMes(LAST_MES_FILE, std::ios::binary | std::ios::in);
    if (fileMes.is_open())
        fileMes.read(reinterpret_cast<char*>(&lastMes), sizeof(lastMes));
    fileMes.close();
}

TgClient::~TgClient() {
    std::fstream fileMes(LAST_MES_FILE, std::ios::binary | std::ios::out);
    if (fileMes.is_open())
        fileMes.write(reinterpret_cast<const char*>(&lastMes), sizeof(lastMes));
    fileMes.close();
}

#undef LAST_MES_FILE
#undef URL_SERVER