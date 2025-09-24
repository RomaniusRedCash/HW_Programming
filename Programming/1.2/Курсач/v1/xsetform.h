#pragma once

#include "xset.h"

class XSetForm {
	XSet* start = nullptr, * cur = nullptr, * recur = nullptr;
	size_t size = 0;
public:
	XSetForm();
	XSetForm(const char* Str);
	//void GetFromF(std::fstream& FI);
	const bool PushEnd(const char& C);
	const bool Next();
	const bool NextNew();
	const bool SetCStart();
	const size_t& GetSize();
	const bool operator==(const char* Str);
	const bool operator==(XSetForm& XSF);
	const bool operator!=(XSetForm& XSF);
	const bool operator<(XSetForm& XSF);
	const bool operator<=(XSetForm& XSF);
	const bool operator>(XSetForm& XSF);
	void clear();
	~XSetForm();

	friend std::ostream& operator<< (std::ostream & O, XSetForm& XSF);
};

