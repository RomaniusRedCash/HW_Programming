#include "ParcerException.h"

const std::string& ParcerException::getMessage() const { return message; }

//std::string TabNotOpen::convertStrToWstr(const std::string& str) {
//	std::string wstr;
//	wstr.resize(str.size());
//	size_t outSize;
//	//mbstowcs_s(&outSize, &wstr[0], str.size() + 1, str.c_str(), str.size());
//	return wstr;
//}
