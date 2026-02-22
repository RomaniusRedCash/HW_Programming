#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "Dater.h"

class Parcer {
protected:
	std::string buffer;
	std::fstream file;
	size_t pos = 0;

	void getLine();
	void openFileUtf(const std::string& path);
	void addDater(Dater& dater, std::vector<std::string> vOut);
public:
	~Parcer();
};

