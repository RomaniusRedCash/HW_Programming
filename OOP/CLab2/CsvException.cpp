#include "CsvException.h"

CsvNotOpen::CsvNotOpen(const std::string& path) {
	message = "CSV tab is not open by path " + path;
}
