#pragma once

#include "xset.h"

class XSetForm {
	XSet* start = nullptr, * cur = nullptr, * recur = nullptr;
	size_t size = 0;
public:
	XSetForm();
	XSetForm(const char* Str);
	XSetForm(const char& Str);
	const bool PushEnd(const char& C);
	XSet* Next();
	XSet* NextNew();
	XSet* SetCStart();
	const size_t& GetSize();
	const char& operator[](const size_t& Id);
	XSetForm& operator=(XSetForm& XSF);
	const bool operator==(const char* Str);
	const bool operator==(XSetForm& XSF);
	const bool operator!=(XSetForm& XSF);
	const bool operator<(XSetForm& XSF);
	const bool operator<=(XSetForm& XSF);
	const bool operator>(XSetForm& XSF);
	void Clear();
	void Prot(std::ostream& O);
	~XSetForm();

	friend std::ostream& operator<< (std::ostream & O, XSetForm& XSF);
};

