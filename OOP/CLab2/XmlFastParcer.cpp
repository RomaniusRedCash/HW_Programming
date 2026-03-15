#include "XmlFastParcer.h"

XmlFastParcer::XmlFastParcer(const std::string& FName, Dater& dater)
{
	file.open(FName, std::ios::in | std::ios::binary);
	file.imbue(std::locale("ru_RU.UTF-8"));


	std::getline(file, buffer);
	std::getline(file, buffer);



	std::vector <std::string> vS = { "city", "street", "house", "floor" };
	for (std::string& i : vS) i = " " + i + "=\"";
	while (true)
	{
		std::getline(file, buffer);
		if (buffer == "</root>") break;
		size_t pos = 0;
		std::vector <std::string> vOut;
		for (std::string& i : vS) {
			pos = buffer.find(i, pos) + i.size();
			size_t end_pos = buffer.find('"', pos);
			vOut.push_back(buffer.substr(pos, end_pos - pos));
		}

		dater.add(vOut[0], vOut[1], std::stoi(vOut[2]), std::stoi(vOut[3]));

	}
}