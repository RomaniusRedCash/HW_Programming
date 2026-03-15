#include "Fun.h"

void PrintX(STR& Str, std::fstream& FO) {
	for (char* i = Str.str; *i != Str.mark; i++) {
		FO << *i;
	}
}

void PrintY(Screen& Scr, std::fstream& FO) {
	for (STR* i = Scr.begin(); i != Scr.end(); i++) {
		if (i->str)
			PrintX(*i, FO);
		FO << std::endl;
	}
}

void Procces(TXT& Text, Screen& Scr, std::fstream& FO) {
	std::setlocale(LC_ALL, "RUSSIAN");
	if (!Text.is_good) {
		FO << "Не верные параметры файла!" << std::endl;
		return;
	}
	ScrData* TempData = new ScrData[Text.GetScaleY()];
	ScrData* MaxS = new ScrData[Text.GetScaleY()];
	for (ScrData* i = MaxS; i < &MaxS[Text.GetScaleY()]; i++) {
		i->str.SetSize(0);
	}
	ScrData MinS;
	MinS.num = std::numeric_limits<size_t>().max();
	size_t Num = 0;
	while (Text.IsNotEnd()) {
		FO << "Блок №" << ++Num << std::endl;
		Text.GetScreen(Scr);
		PrintY(Scr, FO);
		for (size_t i = 0; i < Scr.GetSize() && Scr[i].str; i++) {
			ScrData NowStr;
			NowStr.str.mark = Scr[i].mark;
			NowStr.CreateChild(TempData[i], Scr[i].GetSize());
			char* j = Scr[i].str;
			//for (; *j != Scr[i].mark && NowStr.AddNePov(*j); j++);
			////FO << std::endl;
			////PrintX(Scr[i], FO);
			////FO << std::endl;
			//MaxS = Max(NowStr, MaxS);
			for (; *j != Scr[i].mark; j++) {
				if (!NowStr.AddNePov(*j)) {
					MaxS[i] = Max(NowStr, MaxS[i]);
					//PrintAns(MaxS, FO); std::cout << std::endl;
					NowStr.AddPov(*j);
				}
			}
			MaxS[i] = Max(NowStr, MaxS[i]);
			TempData[i] = NowStr;
		}
		ScrData Temp;
		MiniMax(Temp, MaxS, Scr.GetSize());
		if (Temp.str.str)
			FO << "На момент минимальна ";
		PrintAns(Temp, FO);
		if (Scr.is_last) {
			MiniMax(MinS, MaxS, Scr.GetSize());
			delete[] TempData;
			TempData = new ScrData[Text.GetScaleY()];
			delete[] MaxS;
			MaxS = new ScrData[Text.GetScaleY()];
		}
	}
	FO << "Итого самая малая подстрока из самых больших: ";
	PrintAns(MinS, FO);

	delete[] TempData;
}

void PrintAns(ScrData& ScrD, std::fstream& FO) {
	if (!ScrD.str.str) return;
	for (char* i = &ScrD.str[ScrD.start]; *i != ScrD.str.mark; i++) {
		FO << *i;
		//std::cout << *i;
	}
	FO << " с длинной " << ScrD.num << std::endl;
}

ScrData& Min(ScrData& S1, ScrData& S2) {
	if (S1.num)
		return S1.num < S2.num ? S1 : S2;
	else
		return S2;
}
ScrData& Max(ScrData& S1, ScrData& S2) {
	return S1.num > S2.num ? S1 : S2;
}

void MiniMax(ScrData& S1, ScrData* S2, const size_t& Size) {
	for (size_t i = 0; i < Size; i++) {
		if (S2[i].num) {
			S1 = Min(S1, S2[i]);
		}
	}
}