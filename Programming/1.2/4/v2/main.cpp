#define _CRTDBG_MAP_ALLOC
#include "func.h"
#include <crtdbg.h>

int main() {
	//_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	//_CrtDumpMemoryLeaks();
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//setlocale(LC_ALL, "RUSSIAN");
	std::cout << 'а' << 'ц';
	std::fstream FO("Out.txt", std::ios::out);
	FO << std::boolalpha;
	FO << "Reading:" << std::endl;
	YSetForm YSF1(std::fstream("Inp1.txt", std::ios::in));
	FO << "Inp1.txt:" << std::endl;
	Print(YSF1, FO);
	YSetForm YSF2(std::fstream("Inp2.txt", std::ios::in));
	FO << "Inp2.txt:" << std::endl;
	Print(YSF2, FO);
	YSetForm YSF3(std::fstream("Inp3.txt", std::ios::in));
	FO << "Inp3.txt:" << std::endl;
	Print(YSF3, FO);
	FO << u8"Inp1.txt ⊃ Inp2.txt / Inp3.txt:" << std::endl;
	FO << Proc(YSF1, YSF2, YSF3);
	FO.close();
	return 0;
}