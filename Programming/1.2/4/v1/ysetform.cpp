#include "ysetform.h"

YSetForm::YSetForm(std::fstream FI) {
	SetCStart();
	while (!FI.eof()) {
		cur = new YSet;
		cur->data.GetFromF(FI);
		Next();
	}
}

bool YSetForm::SetCStart() {
	recur = start;
	return cur = start;
}

bool YSetForm::Next() {
	recur = cur;
	return cur = cur->next;
}

YSetForm& YSetForm::operator=(YSetForm& YSF) {
	this->~YSetForm();
	SetCStart();
	YSF.SetCStart();
	do {
		cur->data = YSF.cur->data;
		if (!Next())
			cur = new YSet;
	} while (YSF.Next());
	return *this;
}

YSetForm& YSetForm::operator-(YSetForm& YSF) {
	SetCStart();

}

YSetForm::~YSetForm() {
	SetCStart();
	Next();
	while (recur) {
		delete recur;
		Next();
	}
}

//bool YSetForm::operator==(YSetForm& YSF) {
//	do {
//		if (YSF.cur->data != cur->data || Next() != YSF.Next())
//			return false;
//	} while (cur);
//	return true;
//}

