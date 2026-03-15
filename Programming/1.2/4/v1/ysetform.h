#pragma once

#include "yset.h"

class YSetForm {
	YSet* start = nullptr, * cur = nullptr, * recur = nullptr;
public:
	YSetForm(std::fstream FI);
	bool SetCStart();
	bool Next();
	//bool operator==(YSetForm& YSF);
	YSetForm& operator=(YSetForm& YSF);
	YSetForm& operator-(YSetForm& YSF);
	~YSetForm();
};

