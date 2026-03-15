#include "func.h"
//
//void Print(TXT* Txt, std::fstream& FO) {
//	for (TXT* Cur = Txt; Cur; Cur = Cur->GetNext()) {
//		FO << *Cur << "\n\\/\n";
//		//std::cout << *Cur << "\n\\/\n";
//	}
//	FO << "nullptr" << std::endl;
//}
//
//void Proc(TXT* Txt, TXT* TxtFind) {
//	size_t Count = 0;
//	for (TXT* i = Txt; i; i = i->GetNext()) {
//		if (*i == *TxtFind) Count++;
//	}
//	if (!Count) return;
//	size_t Pos = Count / 2 + Count % 2;
//	Count = 0;
//	TXT* Tmp;
//	if (Pos == 1  && *Txt == *TxtFind) {
//		Tmp = Txt->GetNext();
//		delete Txt;
//		Txt = Tmp;
//		return;
//	}
//	for (TXT* i = Txt; i->GetNext(); i = i->GetNext()) {
//		if (*i->GetNext() == *TxtFind) Count++;
//		if (Count == Pos) {
//			//i->~TXT();
//			Tmp = i->GetNext()->GetNext();
//			delete i->GetNext();
//			i->GetNext() = Tmp;
//			return;
//		}
//	}
//}
//
//void DelTXT(TXT* Txt) {
//	TXT* Cur = Txt, *Tmp;
//	while (Cur) {
//		Tmp = Cur->GetNext();
//		delete Cur;
//		Cur = Tmp;
//	}
//}
bool Proc(YSetForm& YSF1, YSetForm& YSF2, YSetForm& YSF3) {
	YSF2.SetCStart();
	YSF3.SetCStart();
	if (YSF1.SetCStart()) do {
		//bool Brk = false;
		//if (YSF2.GetCur()) do {
		//	if (YSF1.GetCur()->data == YSF2.GetCur()->data) {
		//		Brk = true;
		//		break;
		//	}
		//} while (YSF2.Next() && YSF2.GetCur()->data <= YSF1.GetCur()->data);
		//if (Brk) continue;
		//if(YSF3.GetCur()) do {
		//	if (YSF1.GetCur()->data == YSF3.GetCur()->data) {
		//		Brk = true;
		//		break;
		//	}
		//} while (YSF3.Next() && YSF3.GetCur()->data <= YSF1.GetCur()->data);
		//if (!Brk) return false;
		if (YSF2.GetCur()) while (YSF2.GetCur()->data < YSF1.GetCur()->data && YSF2.Next());
		if (YSF2.GetCur() && YSF2.GetCur()->data == YSF1.GetCur()->data) continue;
		if (!YSF3.GetCur()) return false;
		while (YSF3.GetCur()->data < YSF1.GetCur()->data && YSF3.Next());
		if (!YSF3.GetCur() || YSF3.GetCur()->data != YSF1.GetCur()->data) return false;
	} while (YSF1.Next());
	return true;
}

void Print(YSetForm& YSF, std::fstream& FO) {
	FO << YSF;
}
