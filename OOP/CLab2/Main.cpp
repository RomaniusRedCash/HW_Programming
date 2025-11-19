#include <iostream>
#include <thread>

#include "XmlParcer.h"

int main() {

	setlocale(LC_ALL, "ru_RU.UTF-8");
	std::string pathToFile;
	std::cin >> pathToFile;
	Dater dater;
	XmlParcer(pathToFile, dater);
	dater.printRepeating(std::wcout);
	dater.printFloors(std::wcout);


}
