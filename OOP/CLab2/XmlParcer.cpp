#include "XmlParcer.h"

void XmlParcer::readStr()
{
	buffer.clear();
	wchar_t c;
	while (file.get(c) && c != '\"')
		buffer.push_back(c);
	if (c != '\"') 
		throw "ERROR: Unterminated string";
}

bool XmlParcer::xmlValidate(const wchar_t* expected) {
	std::wstreampos tempPos = file.tellg();
	const size_t len = wcslen(expected);
	buffer.resize(len);
	file.read(&buffer.front(), len);

	if (buffer == expected) return true;
	file.seekg(tempPos);
	return false;
}


XmlParcer::XmlParcer(const std::string& FName, Dater& dater)
{
	file.open(FName, std::ios::in | std::ios::binary);
	file.imbue(std::locale("ru_RU.UTF-8"));
	

	std::getline(file, buffer);
	std::getline(file, buffer);



	std::vector <std::wstring> vS = { L"city", L"street", L"house", L"floor" };
	for (std::wstring& i : vS) i = L" " + i + L"=\"";
	while (true)
	{
		std::getline(file, buffer);
		if (buffer == XML::END_ROOT_STR) break;
		size_t pos = 0;
		std::vector <std::wstring> vOut;
		for (std::wstring& i : vS) {
			pos = buffer.find(i, pos) + i.size();
			size_t end_pos = buffer.find(L'"', pos);
			vOut.push_back(buffer.substr(pos, end_pos - pos));
		}

		dater.add(vOut[0], vOut[1], std::stoi(vOut[2]), std::stoi(vOut[3]));

	}
	std::wcout << L"усё" << std::endl;
}
//XmlParcer::XmlParcer(const std::string& FName, Dater& dater)
//{
//	buffer.reserve(25);
//	// setlocale(LC_ALL, "ru_RU");
//	file.open(FName, std::ios::in | std::ios::binary);
//	// file.imbue(std::locale("C.UTF-8"));
//	file.imbue(std::locale("ru_RU.UTF-8"));
//
//	if (!file.is_open()) throw "Error file open";
//	if (!xmlValidate(XML::XML_STR)) throw "ERROR XML PARCE";
//	if (!xmlValidate(XML::VERSION_STR)) throw "ERROR XML PARCE";
//	readStr();
//
//	if (!xmlValidate(XML::ENCOD_STR)) throw "ERROR XML PARCE";
//	readStr();
//
//	if (!xmlValidate(XML::END_XML_STR)) throw "ERROR XML PARCE";
//	if (!xmlValidate(XML::ROOT_STR)) throw "ERROR XML PARCE";
//
//	while (true)
//	{
//		if (xmlValidate(XML::END_ROOT_STR)) break;
//
//		if(!file.good()) throw "ERROR XML PARCE";
//		if (!xmlValidate(XML::ITEM_STR)) throw "ERROR XML PARCE";
//
//		std::vector <std::wstring> vS = { L"city", L"street", L"house", L"floor" };
//
//		for (std::wstring& i : vS) {
//			i = L" " + i + L"=\"";
//			if (!xmlValidate(i.c_str())) throw "ERROR XML PARCE";
//			readStr();
//			i = buffer;
//		}
//		if (!xmlValidate(XML::END_ITEM_STR)) throw "ERROR XML PARCE";
//
//		dater.add(vS[0], vS[1], std::stoi(vS[2]), std::stoi(vS[3]));
//
//		if (file.fail()) throw "ERROR XML PARCE";
//	}
//	file.close();
//}

XmlParcer::~XmlParcer()
{
	file.close();
}
