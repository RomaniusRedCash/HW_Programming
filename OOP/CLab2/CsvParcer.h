#pragma once

#include <string>
#include <fstream>

#include "Parcer.h"
#include "CsvException.h"

class CsvParcer : public Parcer {
	std::string sepors = { "\";\r" };

	std::string getItem();
public:
	CsvParcer(const std::string& pathToFile, Dater& dater);

};

