#include "xsetform.h"

XSetForm::XSetForm() {
	start = new XSet;
}

XSetForm::XSetForm(const char* Str) {
	start = new XSet;
	SetCStart();
	while (Str[size] != '\0') {
		if (!cur->str.PushEnd(Str[size])) {
			NextNew();
			continue;
		}
		size++;
	}
}

const bool XSetForm::PushEnd(const char& C) {
	while (cur->next) Next();
	if (!cur->str.PushEnd(C)) {
		NextNew();
		cur->str.PushEnd(C);
	}
	size++;
	return true;
}

//void XSetForm::GetFromF(std::fstream& FI) {
//	if (SetCStart()) do {
//		cur->str.Clear();
//	} while (Next());
//	size = 0;
//	SetCStart();
//	char C = '\0';
//	while (FI.get(C) && C != '\n') {
//		if (!cur->str.PushEnd(C)) {
//			NextNew();
//			cur->str.PushEnd(C);
//		}
//		size++;
//	}
//}

const bool XSetForm::Next() {
	recur = cur;
	if (!cur) return false;
	return cur = cur->next;
}

const bool XSetForm::NextNew() {
	if(!cur->next)
		cur->next = new XSet;
	return Next();
}

const bool XSetForm::SetCStart() {
	recur = start;
	return cur = start;
}
const size_t& XSetForm::GetSize() { return size; }

const bool XSetForm::operator==(const char* Str) {
	/*size_t T = 0;
	if (!SetCStart()) return false;
	while (Next())
		for (const char i : recur->str) {
			if (Str[T] != i) return false;
			T++;
		}
	return true;*/
	XSetForm T(Str);
	return *this == T;
}

const bool XSetForm::operator==(XSetForm& XSF) {
	if (XSF.SetCStart() != SetCStart() || size != XSF.size) return false;
	do {
		if (XSF.cur->str != cur->str || Next() != XSF.Next())
			return false;
	} while (cur);
	return true;
}

const bool XSetForm::operator!=(XSetForm& XSF) {
	return !(*this == XSF);
}
const bool XSetForm::operator<(XSetForm& XSF) {
	if (size < XSF.size) return true;
	if (size > XSF.size) return false;
	if (SetCStart() && XSF.SetCStart()) do {
		if (cur->str != XSF.cur->str) return cur->str < XSF.cur->str;
	} while (Next() && XSF.Next());
	return false;
}
const bool XSetForm::operator<=(XSetForm& XSF) {
	return *this < XSF || *this == XSF;
}
const bool XSetForm::operator>(XSetForm& XSF) {
	return !(*this < XSF || *this == XSF);
}
void XSetForm::clear() {
	this->~XSetForm();
	start = new XSet;
}
XSetForm::~XSetForm() {
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