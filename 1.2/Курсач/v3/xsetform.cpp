#include "xsetform.h"

XSetForm::XSetForm() {
	start = new XSet;
}

//XSetForm::XSetForm(XSetForm& XSF) {
//	start = new XSet;
//	*this = XSF;
//}

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

XSetForm::XSetForm(const char& Str) {
	start = new XSet;
	PushEnd(Str);
}

const bool XSetForm::PushEnd(const char& C) {
	if (cur || SetCStart()) while (cur->next) Next();
	if (!cur->str.PushEnd(C)) {
		NextNew()->str.PushEnd(C);
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

XSet* XSetForm::Next() {
	recur = cur;
	if (!cur) return nullptr;
	return cur = cur->next;
}

XSet* XSetForm::NextNew() {
	if(!cur->next)
		cur->next = new XSet;
	return Next();
}

XSet* XSetForm::SetCStart() {
	recur = start;
	return cur = start;
}
const size_t& XSetForm::GetSize() { return size; }

const char& XSetForm::operator[](const size_t& Id) {
	if (Id > size) throw "Out of XSF";
	size_t i = 0;
	if (SetCStart()) do {
		i += cur->str.GetSize();
	} while (Next() && i < Id);
	return recur->str[Id - i + recur->str.GetSize()];
}

XSetForm& XSetForm::operator=(XSetForm& XSF) {
	XSF.SetCStart();
	Clear();
	if (XSF.SetCStart()) do {
		for (const char c : XSF.cur->str) {
			PushEnd(c);
		}
	} while (XSF.Next());
	return *this;
}

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
	if (bool(XSF.SetCStart()) != bool(SetCStart()) || size != XSF.size) return false;
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
	//if (size != XSF.size) return size < XSF.size;
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
void XSetForm::Clear() {
	this->~XSetForm();
	start = new XSet;
	SetCStart();
}
void XSetForm::Prot(std::ostream& O) {
	if (SetCStart()) do {
		O << cur->str << "-->";
	} while (Next());
	O << "nullptr" << std::endl;
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
/*	if (XSF.SetCStart()) do {
			O << XSF.cur->str << "-->";
		} while (XSF.Next());
	return O << "nullptr";*/	
	if (XSF.SetCStart()) do {
			O << XSF.cur->str;
		} while (XSF.Next());
	return O;
}

//std::istream& operator>>(std::istream& I, XSetForm& XSF) {
//	char C = '\0';
//	while (I.get(C) && C != '\n') {
//		XSF.PushEnd(C);
//	}
//	return I;
//}
