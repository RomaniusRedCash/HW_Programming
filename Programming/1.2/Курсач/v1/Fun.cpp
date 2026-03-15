#include "Fun.h"



Menu UserInterf;
CustomSet<Object<Object<PosM>*>> Points, Spos, Times;
CustomSet<Object<PosM>> Marshs;
std::fstream FI;

char C = '\0';
XSetForm XSF;


bool Read() {
	while (FI.get(C)) {
		FI.seekg(-1, std::ios::cur);
		Object<PosM>* MarshObj = GetMarsh();
		if (IsErr(!MarshObj)) return false;

		Object<Object<PosM>*>* PointObj = GetObjFromF(Points);
		if (IsErr(!PointObj || C != '/')) return false;
		Object<Object<PosM>*>* SposObj = GetObjFromF(Points);
		if (IsErr(!SposObj || C != '/')) return false;
		Object<Object<PosM>*>* TimeObj = GetObjFromF(Points);
		if (IsErr(!TimeObj || C == '/')) return false;

		MarshObj->AddInto({ PointObj, SposObj, TimeObj });

		PointObj->AddInto(MarshObj);
		SposObj->AddInto(MarshObj);
		TimeObj->AddInto(MarshObj);

		if (IsErr(!IsClose())) return false;
	}
	return true;
}
Object<PosM>* GetMarsh() {
	XSF.clear();
	SkipSpace();
	if (IsErr(C != '<')) return nullptr;
	while (FI.get(C) && C != '>' && C != '\n') {
		XSF.PushEnd(C);
	}
	if (IsErr(C != '>')) return nullptr;
	return &Marshs.Add(Object<PosM>(XSF));
}
template<typename T>
T* GetObjFromF(CustomSet<T>& CS) {
	XSF.clear();
	SkipSpace();
	//if(IsErr(C != '<')) return nullptr;
	while (FI.get(C) && C != '/' && C != '\n') {
		XSF.PushEnd(C);
	}
	//if (IsErr(C!='/')) return nullptr;
	return &CS.Add(T(XSF));
}

void SkipSpace() {
	while (FI.get(C) && (C == ' ' || C == '\t' || C == '\n'));
	FI.seekg(-1, std::ios::cur);
}

bool IsClose() {
	SkipSpace();
	FI.get(C);
	if (IsErr(C != '<')) return false;
	FI.get(C);
	if (IsErr(C != '/')) return false;
	FI.get(C);
	if (IsErr(C != '>')) return false;
	return true;
}

bool IsErr(const bool& b) {
	if (b) {
		UserInterf.ThrowErr("File is not succes!");
		return true;
	}
	return false;
}

void ShowContent() {
	std::cout << "Routs:" << std::endl;
	size_t Id = 0, SubId = 0;
	for (Content<Object<PosM>> i : Marshs) {
		std::cout << Id++ << "\t" << i.data.GetName() << ':' << std::endl;
		for (Content<PosM> j : i.data.GetSubs()) {
			std::cout << SubId++ << "\t" << j.data.point->GetName() << " - " <<
				j.data.spos->GetName() << " - " <<
				j.data.time->GetName() << std::endl;
		}
	}
}
