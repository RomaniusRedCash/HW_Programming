#pragma once

#include <fstream>
#include <iostream>
#include <iomanip>
#include "STRsClass.h"

class TXT {
	struct PosNSost {
		bool is_end = true;
		std::streampos start = 0;
	};

	char mark = '\0';
	char Temp = ' ';
	size_t scale_x = 0, scale_y = 0;
	std::fstream F;
	PosNSost* lines = nullptr;
	//вычленем вродь из текста а не озу
public:
	bool is_good = true, is_end = false;
	std::streampos end_pos = 0;
	TXT(const char* Name);
	const size_t& GetScaleY();
	bool IsNotEnd();
	void GetScreen(Screen& SCR);
	void MakeScr(Screen& Scr);
	~TXT();
};

struct ScrData {
	STR str;
	size_t num = 0, start = 0, shtraf = 0;
	bool AddNePov(const char& C);
	void AddPov(const char& C);
	void CreateChild(ScrData& Str, size_t NowS);
	void Clear();
	ScrData& operator= (ScrData& Str);
};