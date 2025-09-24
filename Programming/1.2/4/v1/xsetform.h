#pragma once

#include "xset.h"

class XSetForm {
	XSet* start = nullptr, * cur = nullptr;
public:
	XSetForm();
	void GetFromF(std::fstream& FI);
	//XSetForm(XSetForm& XSF);
	XSet* GetCur();
	bool Next();
	bool SetCStart();
	//XSetForm(XSet* Start);
	XSetForm& operator=(XSetForm& XSF);
	bool operator==(XSetForm& XSF);
	bool operator!=(XSetForm& XSF);
	~XSetForm();
};

