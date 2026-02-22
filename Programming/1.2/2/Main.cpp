#include <iostream>
#include <fstream>
#include "STRsClass.h"
#include "TXTClass.h"
#include "Fun.h"

int main() {
	TXT Text("Inp.txt");
	Screen Scr;
	Text.MakeScr(Scr);
	std::fstream FO("Out.txt", std::ios::out);
	Procces(Text, Scr, FO);


	return 0;
}