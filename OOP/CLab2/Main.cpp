#include <iostream>

#include "XmlParcer.h"

int main() {
	std::wstring path;
	std::wcin >> path;
	Dater dater;
	XmlParcer(path, dater);
	
	

	//setlocale(LC_ALL, "ru_RU");

	//std::wfstream F("123.txt", std::ios::in);
	//F.imbue(std::locale("ru_RU.UTF-8"));
	//wchar_t C;
	//while (F.get(C)){
	//	std::wstreampos pos = F.tellg();
	//	std::wcout <<C<<"\t" << F.tellg() << std::endl;
	//	F.get(C);
	//	F.seekg(pos);
	//	F.clear();
	//}

}