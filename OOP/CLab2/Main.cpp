#include <iostream>
#include <thread>

#include "XmlParcer.h"

int main() {

	setlocale(LC_ALL, "ru_RU");
	std::string pathToFile;
	std::cin >> pathToFile;
	Dater dater;
	XmlParcer(pathToFile, dater);
	dater.printRepeating(std::wcout);
}
