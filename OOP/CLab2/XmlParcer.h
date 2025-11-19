#pragma once

// #define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
//#define _SILENCE_LOCALE_EMPTY_DEPRECATION_WARNING

#include <fstream>
#include <string>
#include <vector>
#include <locale>
#include <codecvt>
#include <sstream>

#include "Dater.h"

namespace XML {
	const wchar_t XML_STR[] = L"<?xml";
	const wchar_t VERSION_STR[] = L" version=\"";
	const wchar_t ENCOD_STR[] = L" encoding=\"";
	const wchar_t END_XML_STR[] = L"?>\n";
	const wchar_t ROOT_STR[] = L"<root>\n";
	const wchar_t END_ROOT_STR[] = L"</root>";
	const wchar_t ITEM_STR[] = L"<item";
	const wchar_t END_ITEM_STR[] = L" />\n";
}

class XmlParcer
{
	std::wstring buffer;
	std::wfstream file;
	void readStr();
	bool xmlValidate(const wchar_t* expected);

public:
	XmlParcer(const std::string& FName, Dater& dater);
	~XmlParcer();
};

