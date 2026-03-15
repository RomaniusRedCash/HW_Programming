#pragma once

#include <string>
#include <locale>
#include <codecvt>

class ParcerException {
protected:
	std::string message;
public:
	const std::string& getMessage() const;
};

class TabNotOpen : public ParcerException {
protected:
	//std::string convertStrToWstr(const std::string& str);
public:
};