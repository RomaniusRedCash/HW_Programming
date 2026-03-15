#include "Parcer.h"

void Parcer::getLine() {
	pos = 0;
	std::getline(file, buffer);
}

void Parcer::openFileUtf(const std::string& path) {
	file.open(path, std::ios::in);
	file.imbue(std::locale("ru_RU.UTF-8"));
}

void Parcer::addDater(Dater& dater, std::vector<std::string> vOut) {
	dater.add(vOut[0], vOut[1], std::stoi(vOut[2]), std::stoi(vOut[3]));
}

Parcer::~Parcer() {
	file.close();
}
