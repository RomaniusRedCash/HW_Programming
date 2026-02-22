//#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include "Menu.h"
#include "fstream"



//Menu UserInterf;
//SetOfObj Points, Spos, Times, Marshs;
//SetOfObj Points, Spos, Times;
//MarshList Marshs;
std::fstream FI("Inp.txt", std::ios::in), FProt("Prot.txt", std::ios::out);
Menu UI;

int main() {
	FProt << "a";
	//_CrtSetBreakAlloc(191);
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
	UI.Read();
	UI.ProtAll();
	////ShowMarshs();
	while (UI.GetSost() != Sost::Exit && UI.GetSost() != Sost::Err)
		UI.AnalizIn();
	system("pause");
	return 0;
}


