#include <iostream>

#include "XmlParcer.h"
#include "CsvParcer.h"
#include "Timer.h"

int main() {
	setlocale(LC_ALL, "ru_RU.UTF-8");

	std::string pathToFile;
	while (true) {
		std::cout << "Введите путь к файлу в формата .xml или .csv:" << std::endl;
		std::cin >> pathToFile;
		if (pathToFile == "stop") break;
		Dater dater;
		if (pathToFile.size() > 4) {
			if (pathToFile.compare(pathToFile.size() - 4, 4, ".xml") == 0) {
				try {
					Timer timer(std::cout);
					XmlParcer Parcer(pathToFile, dater);
				}
				catch (const ParcerException& e) {
					std::cout << e.getMessage() << std::endl;
				}
			}
			else if (pathToFile.compare(pathToFile.size() - 4, 4, ".csv") == 0) {
				try {
					Timer timer(std::cout);
					CsvParcer Parcer(pathToFile, dater);
				}
				catch (const ParcerException& e) {
					std::cout << e.getMessage() << std::endl;
				}
			} else continue;
			std::cout << "Повторяющиеся строки:" << std::endl;
			dater.printRepeating(std::cout);

			std::cout << "Этажей в городах:" << std::endl;
			dater.printFloors(std::cout);

			std::cout << std::endl;
			std::cout << std::endl;
			std::cout << std::endl;
		}
		else std::cout << "Ввод некорректен!" << std::endl;
	}
	
}
