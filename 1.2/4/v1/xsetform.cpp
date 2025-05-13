#include "xsetform.h"

//XSetForm::XSetForm(XSet* Start) {
//	start = Start;
//	//end = Start;
//	//while (end->next) {
//	//	end = end->next;
//	//}
//}

XSetForm::XSetForm() {
	start = new XSet;
}

void XSetForm::GetFromF(std::fstream& FI) {
	this->~XSetForm();
	start = new XSet;
	SetCStart();
	char C = '\0';
	while (FI.get(C) && C != '\n') 
		if (!cur->str.PushEnd(C)) {
			FI.seekg(-1, std::ios::cur);
			if (!Next())
				cur = new XSet;
		}
}

//XSetForm::XSetForm(XSetForm& XSF) {
//	start = new XSet;
//	*this = XSF;
//}

XSet* XSetForm::GetCur() {
	return cur;
}

bool XSetForm::Next() {
	return cur = cur->next;
}

bool XSetForm::SetCStart() {
	return cur = start;
}

XSetForm& XSetForm::operator=(XSetForm& XSF) {
	this->~XSetForm();
	SetCStart();
	XSF.SetCStart();
	do {
		cur->str = XSF.cur->str;
		if (!Next())
			cur = new XSet;
	} while (XSF.Next());
	return *this;
}

bool XSetForm::operator==(XSetForm& XSF) {
	do {
		if (XSF.cur->str != cur->str || Next() != XSF.Next())
			return false;
	} while (cur);
	return true;
}

bool XSetForm::operator!=(XSetForm& XSF) {
	return !(*this == XSF);
}

XSetForm::~XSetForm() {
	SetCStart();
	XSet* old;
	do {
		old = cur;
		delete old;
	} while (Next());
}
