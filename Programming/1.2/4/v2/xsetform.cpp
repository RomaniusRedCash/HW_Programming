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
	if (SetCStart()) do {
		cur->str.Clear();
	} while (Next());
	SetCStart();
	char C = '\0';
	while (FI.get(C) && C != '\n') 
		if (!cur->str.PushEnd(C)) {
			NextNew();
			cur->str.PushEnd(C);
		}
}
//
//XSetForm::XSetForm(XSetForm& XSF) {
//	start = new XSet;
//	*this = XSF;
//}
//
//XSet* XSetForm::GetCur() {
//	return cur;
//}
//
bool XSetForm::Next() {
	recur = cur;
	if (!cur) return false;
	return cur = cur->next;
}

bool XSetForm::NextNew() {
	if(!cur->next)
		cur->next = new XSet;
	return Next();
}

bool XSetForm::SetCStart() {
	recur = start;
	return cur = start;
}
const size_t XSetForm::GetSize() {
	size_t Size = 0;
	if (SetCStart()) do {
		Size += cur->str.GetSize();
	} while (Next());
	return Size;
}
//
//XSetForm& XSetForm::operator=(XSetForm& XSF) {
//	this->~XSetForm();
//	SetCStart();
//	XSF.SetCStart();
//	do {
//		cur->str = XSF.cur->str;
//		if (!Next())
//			cur = new XSet;
//	} while (XSF.Next());
//	return *this;
//}
//
bool XSetForm::operator==(XSetForm& XSF) {
	if (XSF.SetCStart() != SetCStart()) return false;
	do {
		if (XSF.cur->str != cur->str || Next() != XSF.Next())
			return false;
	} while (cur);
	return true;
}
bool XSetForm::operator<(XSetForm& XSF) {
	size_t S1 = GetSize(), S2 = XSF.GetSize();
	if (S1 < S2) return true;
	if (S1 > S2) return false;
	if (SetCStart() && XSF.SetCStart()) do {
		if (cur->str < XSF.cur->str) return true;
		if (cur->str > XSF.cur->str) return false;
	} while (Next() && XSF.Next());
	return false;
}
bool XSetForm::operator>(XSetForm& XSF) {
	return !(*this < XSF || *this == XSF);
}
//bool XSetForm::operator>=(XSetForm& XSF) {
//	return !(*this < XSF) || *this == XSF;
//}
//
//bool XSetForm::operator!=(XSetForm& XSF) {
//	return !(*this == XSF);
//}
//
XSetForm::~XSetForm() {
	//std::cout << "Deleting X" << std::endl;
	SetCStart();
	Next();
	while (recur) {
		delete recur;
		Next();
	}
}

std::ostream& operator<<(std::ostream& O, XSetForm& XSF) {
	if (XSF.SetCStart()) do {
			O << XSF.cur->str << "-->";
		} while (XSF.Next());
	return O << "nullptr";
}
