#include "CsvParcer.h"

std::string CsvParcer::getItem() {
	size_t startPos = buffer.find_first_not_of(sepors, pos);
	size_t endPos = buffer.find_first_of(sepors, startPos);
	pos = buffer.find_first_not_of(sepors, endPos);
	if (endPos > buffer.size()) {
		endPos = buffer.size();
	}
	return buffer.substr(startPos, endPos - startPos);
}

CsvParcer::CsvParcer(const std::string& pathToFile, Dater& dater) {
	openFileUtf(pathToFile);
	if (!file.is_open()) throw CsvNotOpen(pathToFile);
	getLine();
	while (!file.eof()) {
		getLine();
		if (buffer.empty()) break;
		std::vector<std::string> vOut;
		vOut.reserve(4);
		for (size_t i = 0; i < 4; i++)
			vOut.push_back(getItem());
		addDater(dater, vOut);
	}
}
