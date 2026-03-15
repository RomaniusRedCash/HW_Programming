#include "ysetform.h"
//
//YSetForm::YSetForm(std::fstream FI) {
//	if (!FI.is_open()) throw "File is not open!\n";
//	start = new YSet;
//	SetCStart();
//	char C;
//	if (FI.get(C)) while (true) {
//		FI.seekg(-1, std::ios::cur);
//		cur->data.GetFromF(FI);
//		if (!FI.get(C))
//			break;
//		NextNew();
//	}
//	//if (start == nullptr) throw "Some file is empty!\n";
//	FI.close();
//}
YSetForm::YSetForm(std::fstream FI) {
	if (!FI.is_open()) throw "File is not open!\n";
	start = new YSet;
	char C;
	if (!FI.get(C)) return;
	FI.seekg(-1, std::ios::cur);
	start->data.GetFromF(FI);
	while (FI.get(C)) {
		FI.seekg(-1, std::ios::cur);
		YSet* Tmp = new YSet;
		Tmp->data.GetFromF(FI);
		if (Tmp->data < start->data) {
			Tmp->next = start;
			start = Tmp;
			continue;
		} else if (start->data == Tmp->data) {
			delete Tmp;
			continue;
		}
		SetCStart();
		while (Next() && Tmp->data > cur->data);
		if (!cur) {
			recur->next = Tmp;
			cur = Tmp;
			continue;
		}
		if (Tmp->data < cur->data) {
			recur->next = Tmp;
			Tmp->next = cur;
			continue;
		}
		//if (Tmp->data > cur->data) {
		//	cur->next = Tmp;
		//	continue;
		//}
		delete Tmp;
	}
	//if (start == nullptr) throw "Some file is empty!\n";
	FI.close();
}

bool YSetForm::SetCStart() {
	recur = start;
	return cur = start;
}

bool YSetForm::Next() {
	recur = cur;
	if (!cur) return false;
	return cur = cur->next;
}

bool YSetForm::NextNew() {
	if(!cur->next)
		cur->next = new YSet;
	return Next();
}

YSet* YSetForm::GetCur() {
	return cur;
}
//
//YSetForm& YSetForm::operator=(YSetForm& YSF) {
//	this->~YSetForm();
//	SetCStart();
//	YSF.SetCStart();
//	do {
//		cur->data = YSF.cur->data;
//		if (!Next())
//			cur = new YSet;
//	} while (YSF.Next());
//	return *this;
//}
//
//YSetForm& YSetForm::operator-(YSetForm& YSF) {
//	SetCStart();
//	return *this;
//}
//
YSetForm::~YSetForm() {
	//std::cout << "Deleting Y" << std::endl;
	SetCStart();
	Next();
	while (recur) {
		delete recur;
		Next();
	}
}
//
//bool YSetForm::operator==(YSetForm& YSF) {
//	do {
//		if (YSF.cur->data != cur->data || Next() != YSF.Next())
//			return false;
//	} while (cur);
//	return true;
//}
//
std::ostream& operator<<(std::ostream& O, YSetForm& YSF) {
	if (YSF.SetCStart()) do {
			O << YSF.cur->data << "\n\\/\\/\\/\n";
		} while (YSF.Next());
	O << "nullptr" << std::endl;
	return O;
}
