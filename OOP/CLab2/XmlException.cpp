#include "XmlException.h"

XmlExItem::XmlExItem(const std::string& name) {
	message = "Not found " + name;
}

XmlValidate::XmlValidate(const std::string& xmlStr) {
	message = "file dont contain " + xmlStr;
}

XmlNotOpen::XmlNotOpen(const std::string& path) {
	message = "XML tab is not open by path " + path;
}
