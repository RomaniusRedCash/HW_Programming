#include "str.h"

STR::STR() {
	str = new char[const_size + 1];
	str[0] = mark;
}

void STR::Clear() {
	size = 0;
	str[0] = mark;
}

bool STR::PushEnd(const char& C) {
	for (char& i : *this)
		if (i == mark) {
			size++;
			i = C;
			*(&i + 1) = mark;
			return true;
		}
	return false;
}

const size_t& STR::GetSize() { return size; }

char& STR::operator[] (const size_t& Ind) {
	if (Ind > size || Ind < 0) throw "Out of STR.";
	return str[Ind];
}

bool STR::operator==(STR& Str) {
	if (size != Str.GetSize()) return false;
	for (size_t i = 0; i < size; i++) {
		if (str[i] != Str[i]) return false;
	}
	return true;
}

bool STR::operator!=(STR& Str) {
	return !(*this == Str);
}

bool STR::operator<(STR& Str) {
	if (size > Str.size) return false;
	for (size_t i = 0; i < size; i++) {
		if (str[i] != Str[i]) return str[i] < Str[i];
	}
	return true;
}
bool STR::operator>(STR& Str) {
	return !(*this < Str || *this == Str);
}

char* STR::begin() const { return str; }

char* STR::end() const { return &str[const_size]; }

STR::~STR() {
	size = 0;
	if (str) delete[] str;
}

std::ostream& operator<<(std::ostream& Os, STR& Str) { return Os << Str.str; }
