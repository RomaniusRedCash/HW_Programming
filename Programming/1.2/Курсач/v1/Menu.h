#pragma once

#include "xsetform.h"

class Menu {
	enum Sost : unsigned short {
		Main, Help, Choose, Make, IsErr
	} sost = Sost::Main;
public:
	void ThrowErr(const char* IsErr);
	void AnalizIn(const char* Set);

};

