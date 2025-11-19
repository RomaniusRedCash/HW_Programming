#include <iostream>
#include <thread>

#include "XmlParcer.h"

int main() {
	//std::locale::global(std::locale("ru_RU.UTF-8"));
	setlocale(LC_ALL, "ru_RU.UTF-8");
	//std::wcout.imbue(std::locale("ru_RU.UTF-8"));
	std::string pathToFile;
	std::cin >> pathToFile;
	Dater dater;
	XmlParcer(pathToFile, dater);
	std::wcout<<L"Считано"<<std::endl;
	dater.printRepeating(std::wcout);
	dater.printFloors(std::wcout);

	system("pause");
}
