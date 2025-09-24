#include "xsetform.h"

XSetForm::XSetForm() {
	start = new XSet;
}

void XSetForm::GetFromF(std::fstream& FI) {
	if (SetCStart()) do {
		cur->str.Clear();
	} while (Next());
	size = 0;
	SetCStart();
	char C = '\0';
	while (FI.get(C) && C != '\n') {
		if (!cur->str.PushEnd(C)) {
			NextNew();
			cur->str.PushEnd(C);
		}
		size++;
	}
}
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
const size_t& XSetForm::GetSize() { return size; }

bool XSetForm::operator==(XSetForm& XSF) {
	if (XSF.SetCStart() != SetCStart() || size != XSF.size) return false;
	do {
		if (XSF.cur->str != cur->str || Next() != XSF.Next())
			return false;
	} while (cur);
	return true;
}
bool XSetForm::operator!=(XSetForm& XSF) {
	return !(*this == XSF);
}
bool XSetForm::operator<(XSetForm& XSF) {
	if (size < XSF.size) return true;
	if (size > XSF.size) return false;
	if (SetCStart() && XSF.SetCStart()) do {
		if (cur->str != XSF.cur->str) return cur->str < XSF.cur->str;
	} while (Next() && XSF.Next());
	return false;
}
bool XSetForm::operator<=(XSetForm& XSF) {
	return *this < XSF || *this == XSF;
}
bool XSetForm::operator>(XSetForm& XSF) {
	return !(*this < XSF || *this == XSF);
}
XSetForm::~XSetForm() {
	//std::cout << "Deleting X" << std::endl;
	size = 0;
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
//
//XSetForm::XSetForm(XSet* Start) {
//	start = Start;
//	//end = Start;
//	//while (end->next) {
//	//	end = end->next;
//	//}
//}
//
//bool XSetForm::operator>=(XSetForm& XSF) {
//	return !(*this < XSF) || *this == XSF;
//}
//
//bool XSetForm::operator!=(XSetForm& XSF) {
//	return !(*this == XSF);
//}
//
