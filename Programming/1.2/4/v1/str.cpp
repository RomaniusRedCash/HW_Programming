#include "str.h"

STR::STR() {
	str = new char[const_size + 1];
	str[0] = mark;
}

const size_t& STR::GetSize() const {
	for (size_t i = 0; i < const_size; i++)
		if (str[i] == mark) return i;
}

bool STR::PushEnd(const char& C) {
	for (char& i : *this)
		if (i == mark) {
			i = C;
			*(&i + 1) = mark;
			return true;
		}
	return false;
}

char& STR::operator[] (const size_t& Ind) {
	if (Ind > const_size || Ind < 0) throw "Out of STR.";
	return str[Ind];
}

bool STR::operator==(STR& Str) {
	if (const_size != Str.GetSize()) return false;
	for (size_t i = 0; i < const_size; i++) {
		if (str[i] != Str[i]) return false;
		if (str[i] == Str[i] && str[i] == mark)	break;
	}
	return true;
}

bool STR::operator!=(STR& Str) {
	return !(*this == Str);
}

STR& STR::operator=(STR& Str) {
	for (size_t i = 0; i < const_size; i++)
		str[i] = Str[i];
	return *this;
}

//STR::operator bool() const {
//	return str;
//}

char* STR::begin() const { return str; }

char* STR::end() const { return &str[const_size]; }

STR::~STR() {
	if (str)
		delete[] str;
}

std::ostream& operator<<(std::ostream& Os, STR& Str) { return Os << Str.str; }