#pragma once

#include "xset.h"

class XSetForm {
	XSet* start = nullptr, * cur = nullptr, * recur = nullptr;;
public:
	XSetForm();
	void GetFromF(std::fstream& FI);
	//XSetForm(XSetForm& XSF);
	//XSet* GetCur();
	bool Next();
	bool NextNew();
	bool SetCStart();
	const size_t GetSize();
	//XSetForm(XSet* Start);
	//XSetForm& operator=(XSetForm& XSF);
	bool operator==(XSetForm& XSF);
	bool operator<(XSetForm& XSF);
	bool operator>(XSetForm& XSF);
	//bool operator>=(XSetForm& XSF);
	//bool operator!=(XSetForm& XSF);
	~XSetForm();

	friend std::ostream& operator<< (std::ostream & O, XSetForm& XSF);
};

