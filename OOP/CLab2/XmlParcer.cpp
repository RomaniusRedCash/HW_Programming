#include "XmlParcer.h"

std::string XmlParcer::readItem(const std::string& item) {
	pos = buffer.find(item, pos) + item.size();
	size_t end = buffer.find('"', pos);
	return buffer.substr(pos, end - pos);
}



bool XmlParcer::xmlValidate(const char* expected) {
	pos = buffer.find(expected, pos);
	if (pos == std::string::npos) throw XmlValidate(expected);
	return true;
}

XmlParcer::XmlParcer(const std::string& pathToFile, Dater& dater)
{
	for (std::string& i : vS)
		i = ' ' + i + "=\"";

	openFileUtf(pathToFile);
	if (!file.is_open()) throw XmlNotOpen(pathToFile);

	getLine();

	xmlValidate(XML::XML_STR);

	readItem(XML::VERSION_STR);
	readItem(XML::ENCOD_STR);

	xmlValidate(XML::END_XML_STR);

	getLine();
	xmlValidate(XML::ROOT_STR);

	while (!file.eof())
	{
		getLine();

		size_t oldPos = pos;
		if (buffer.find(XML::END_ROOT_STR, pos) != std::string::npos) break;
		pos = oldPos;

		if(!file.good()) throw "ERROR XML PARCE";

		xmlValidate(XML::ITEM_STR);

		std::vector <std::string> vOut;
		vOut.reserve(vS.size());
		for (std::string& i : vS) {
			xmlValidate(i.c_str());
			vOut.push_back(readItem(i));
		}
		xmlValidate(XML::END_ITEM_STR);

		addDater(dater, vOut);

		if (file.fail()) throw "ERROR XML PARCE";
	}
	file.close();
}
