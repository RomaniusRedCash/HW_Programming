#pragma once

#include <fstream>

#include "Parcer.h"

class XmlFastParcer : public Parcer {
public:
	XmlFastParcer(const std::string& FName, Dater& dater);
};

