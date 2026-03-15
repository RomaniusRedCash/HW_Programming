#include "txt.h"

STR& TXT::GetStr() { return str; }

void TXT::GetFromTXT(std::fstream& FI) {
	std::streampos Pos = FI.tellg();
	if (!FI.is_open()) throw "File is not open.";
	char C;
	size_t Size = 0;
	while (FI.get(C) && C != '\n')
		Size++;
	str.SetSize(Size);
	FI.clear();
	FI.seekg(Pos);
	Size = 0;
	while (FI.get(C) && C != '\n')
		str[Size++] = C;
	if (FI.get(C)) {
		FI.seekg(-1, std::ios::cur);
		next = new TXT;
		next->GetFromTXT(FI);
	}
}

//bool TXT::IsNext() { return next; }

TXT*& TXT::GetNext() { return next; }

//TXT* TXT::operator++() { 
//	Next(this, next); 
//	return this;
//}

bool TXT::operator==(TXT& Txt) { return str == Txt.str; }

//void Next(TXT* Txt1, TXT* Txt2) { Txt1 = Txt2->next; }

std::ostream& operator<<(std::ostream& Os, TXT& Txt) { return Os << Txt.str; }

//TXT::~TXT() { str.~STR(); }

