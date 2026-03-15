#pragma once

// #define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
//#define _SILENCE_LOCALE_EMPTY_DEPRECATION_WARNING



#include "Parcer.h"
#include "XmlException.h"

namespace XML {
	const char XML_STR[] = "<?xm";
	const char VERSION_STR[] = " version=\"";
	const char ENCOD_STR[] = " encoding=\"";
	const char END_XML_STR[] = "?>";
	const char ROOT_STR[] = "<root>";
	const char END_ROOT_STR[] = "</root>";
	const char ITEM_STR[] = "<item";
	const char END_ITEM_STR[] = " />";
}

class XmlParcer : public Parcer {
	std::vector <std::string> vS = { "city", "street", "house", "floor" };

	std::string readItem(const std::string& item);
	bool xmlValidate(const char* expected);

public:
	XmlParcer(const std::string& FName, Dater& dater);
};

