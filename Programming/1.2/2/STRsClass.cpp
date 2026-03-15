#include <iostream>
#include "STRsClass.h"

size_t STR::GetSize() {
	if (!str) return 0;
	size_t Size = 0;
	for (char* i = str; *i != mark; i++, Size++);
	return Size;
}

char& STR::operator[] (const size_t& i) {
	return str[i];
}
//STR::STR(const size_t& Size) : str(new char[Size+1]) {
//	str[Size] = mark;
//}
void STR::SetSize(const size_t& Size) {
	delete[] str;
	str = new char[Size + 1];
	str[Size] = mark;
}

//bool STR::AddNePov(const char C) {
//	char* i = str;
//	for (; *i != mark; i++) {
//		if (*i == C) return false;
//	}
//	*i = C;
//	*(i + 1) = mark;
//	return true;
//}
//
//void STR::CreateChild(STR& Str, size_t NowS) {
//	if (!Str.str) return;
//	SetSize(NowS + Str.GetSize());
//	for (size_t i = 0; Str[i] != mark; i++) {
//		str[i] = Str[i];
//	}
//}

STR::~STR() {
	delete[] str;
}



const size_t& Screen::GetSize() {
	return size;
};


STR& Screen::operator[] (const size_t& i) {
	return strs[i];
}

//STR& STR::operator== (STR& Str) {
//	this->~STR();
//	SetSize(Str.GetSize());
//	for (size_t i = 0; Str[i] != mark; i++) {
//		str[i] = Str[i];
//	}
//}

void Screen::NewScr() {
	for (size_t i = 0; i < size; i++) {
		strs[i].~STR();
		strs[i].str = nullptr;
	}
	is_last = false;
}

void Screen::NewScr(const char& Mark, const size_t& Size) {
	size = Size;
	this->~Screen();
	strs = new STR[size];
	for (size_t i = 0; i < size; i++) {
		strs[i].mark = Mark;
	}
}

STR* Screen::end() {
	return &strs[size];
}

STR* Screen::begin() {
	return strs;
}

Screen::~Screen() {
	delete[] strs;
}



//static std::iostream& operator<< (std::iostream& FS, Screen& Scr) {
//	for (size_t i = 0; i < Scr.GetSize(); i++)
//		for (char* j = Scr[i].str; *j != Scr[i].mark; j++) FS << *j;
//	return FS;
//}