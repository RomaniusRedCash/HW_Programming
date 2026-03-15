#pragma once

#include "yset.h"

class YSetForm {
	YSet* start = nullptr, * cur = nullptr, * recur = nullptr;
public:
	YSetForm(std::fstream FI);
	bool SetCStart();
	bool Next();
	bool NextNew();
	//bool operator==(YSetForm& YSF);
	YSet* GetCur();
	//YSetForm& operator=(YSetForm& YSF);
	//YSetForm& operator-(YSetForm& YSF);
	~YSetForm();

	friend std::ostream& operator<<(std::ostream& O, YSetForm& YSF);
};

