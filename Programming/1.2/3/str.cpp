#include "str.h"

void STR::SetSize(const size_t& Size) {
	if (str)
		delete[] str;
	size = Size;
	str = new char[size + 1];
	str[size] = mark;
}

const size_t& STR::GetSize() const {
	return size;
}

char& STR::operator[] (const size_t& Ind) {
	if (Ind > size || Ind < 0) throw "Out of STR.";
	return str[Ind];
}

bool STR::operator==(STR& Str) {
	if (size != Str.GetSize()) return false;
	for (size_t i = 0; i < size; i++)
		if (str[i] != Str[i]) return false;
	return true;
}

//STR::operator bool() const {
//	return str;
//}

char* STR::begin() const { return str; }

char* STR::end() const { return &str[size]; }

STR::~STR() {
	if (str)
		delete[] str;
	size = 0;
}

std::ostream& operator<<(std::ostream& Os, STR& Str) { return Os << Str.str; }