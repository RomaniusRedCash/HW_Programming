#include <crtdbg.h>
#include "Menu.h"
#include "fstream"

std::fstream FI("Inp.txt", std::ios::in), FProt("Prot.txt", std::ios::out);
Menu UI;

int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
	UI.Read();
	UI.ProtAll();
	while (UI.GetSost() != Sost::Exit && UI.GetSost() != Sost::Err)
		UI.AnalizIn();
	system("pause");
	return 0;
}


