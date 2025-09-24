#pragma once

#include "str.h"

class TXT {
	TXT* next = nullptr;
	STR str;
	//friend void Next(TXT* Txt1, TXT* Txt2);
public:
	STR& GetStr();
	void GetFromTXT(std::fstream& FI);
	//bool IsNext();
	TXT*& GetNext();
	//TXT* operator++();
	bool operator==(TXT& Txt);
	friend std::ostream& operator<<(std::ostream& Os, TXT& Txt);
	//~TXT();
};