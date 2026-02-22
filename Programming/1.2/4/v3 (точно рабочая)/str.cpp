#include "str.h"

STR::STR() {
	str = new char[const_size + 1];
	size = 0;
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
	//std::cout << "Deleting str " << str << std::endl;
	size = 0;
	if (str) delete[] str;
}

std::ostream& operator<<(std::ostream& Os, STR& Str) { return Os << Str.str; }
//
//const size_t& STR::GetSize() const {
//	for (size_t i = 0; i < const_size; i++)
//		if (str[i] == mark) return i;
//	return 0;
//}
//
//STR& STR::operator=(STR& Str) {
//	for (size_t i = 0; i < const_size; i++)
//		str[i] = Str[i];
//	return *this;
//}
//
//STR::operator bool() const {
//	return str;
//}
//