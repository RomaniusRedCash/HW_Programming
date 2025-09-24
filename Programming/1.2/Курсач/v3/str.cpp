#include "str.h"

STR::STR() {
	str = new char[const_size + 1];
	str[0] = mark;
	last = str;
}

void STR::Clear() {
	//size = 0;
	str[0] = mark;
	last = str;
}

bool STR::PushEnd(const char& C) {
	if (GetSize() < const_size) {
		*last++ = C;
		*last = mark;
		return true;
	}
	return false;
	/*for (char& i : *this)
		if (i == mark) {
			size++;
			i = C;
			*(&i + 1) = mark;
			return true;
		}
	return false;*/
}

const size_t STR::GetSize() { return last-str+1; }

char& STR::operator[] (const size_t& Ind) {
	if (Ind > GetSize() || Ind < 0) throw "Out of STR.";
	return str[Ind];
}

bool STR::operator==(STR& Str) {
	if (GetSize() != Str.GetSize()) return false;
	for (size_t i = 0; i < GetSize(); i++) {
		if (str[i] != Str[i]) return false;
	}
	return true;
}

bool STR::operator!=(STR& Str) {
	return !(*this == Str);
}

bool STR::operator<(STR& Str) {
	//if (GetSize() > Str.GetSize()) return false;
	for (size_t i = 0; i < GetSize(); i++) {
		if (str[i] != Str[i]) return str[i] < Str[i];
	}
	return true;
}
bool STR::operator>(STR& Str) {
	return !(*this < Str || *this == Str);
}

char* STR::begin() const { return str; }

char* STR::end() const { return last; }

STR::~STR() {
	last = nullptr;
	//size = 0;
	if (str) delete[] str;
}

std::ostream& operator<<(std::ostream& Os, STR& Str) { 
	return Os << Str.str; 
}
