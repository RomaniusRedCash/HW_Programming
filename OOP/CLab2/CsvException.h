#pragma once

#include "ParcerException.h"

class CsvNotOpen : public TabNotOpen {
public:
	CsvNotOpen(const std::string& path);
};

