#pragma once

#include "xset.h"

class XSetForm {
	XSet* start = nullptr, * cur = nullptr, * recur = nullptr;
	size_t size = 0;
public:
	XSetForm();
	void GetFromF(std::fstream& FI);

	bool Next();
	bool NextNew();
	bool SetCStart();
	const size_t& GetSize();
	bool operator==(XSetForm& XSF);
	bool operator!=(XSetForm& XSF);
	bool operator<(XSetForm& XSF);
	bool operator<=(XSetForm& XSF);
	bool operator>(XSetForm& XSF);
	~XSetForm();
	//XSetForm(XSetForm& XSF);
	//XSet* GetCur();
	//XSetForm(XSet* Start);
	//XSetForm& operator=(XSetForm& XSF);
	//bool operator>=(XSetForm& XSF);
	//bool operator!=(XSetForm& XSF);
	friend std::ostream& operator<< (std::ostream & O, XSetForm& XSF);
};

