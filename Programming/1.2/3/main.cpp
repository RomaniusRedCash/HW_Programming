#define _CRTDBG_MAP_ALLOC
#include "func.h"
#include <crtdbg.h>

int main() {
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	std::fstream FI1("Inp1.txt", std::ios::in), FI2("Inp2.txt", std::ios::in), FO("Out.txt", std::ios::out);
	TXT *Txt, *TxtFind;
	Txt = new TXT;
	TxtFind = new TXT;
	Txt->GetFromTXT(FI1);
	TxtFind->GetFromTXT(FI2);
	FO << "Исходные строки" << std::endl;
	Print(Txt, FO);
	FO << "Удалить" << std::endl;
	Print(TxtFind, FO);
	Proc(Txt, TxtFind);
	FO << "Итог" << std::endl;
	Print(Txt, FO);
	DelTXT(Txt);
	//TxtFind->~TXT();
	DelTXT(TxtFind);
	_CrtDumpMemoryLeaks();
	return 0;
}