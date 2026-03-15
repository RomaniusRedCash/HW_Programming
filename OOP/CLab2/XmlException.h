#pragma once

#include "ParcerException.h"

class XmlExItem : public ParcerException {
public:
	XmlExItem(const std::string& name);
};

class XmlValidate : public ParcerException {
public:
	XmlValidate(const std::string& xmlStr);
};

class XmlNotOpen : public TabNotOpen {
public:
	XmlNotOpen(const std::string& path);
};
