#pragma once

#include "xsetform.h"
#include "Object.h"

enum Sost : unsigned short {
	Main, Help, Show, Select, Make, Err, Exit
};

class Menu {
	SetOfObj Marshs;
	SetOfObj AlfPoints, AlfSpos, AlfTimes;
	Sost sost = Sost::Main;
	char C = '\0';
	XSetForm XSF;
public:
	const Sost& GetSost();
	void ThrowErr(const char* IsErr);
	bool AnalizIn();

	bool Read();
	void ProtAll();
	void ProtSO(SetOfObj& SO);

	void GetXSF();
	Object* GetMarsh();
	Object* GetObjFromF(SetOfObj& Alf);
	Object* InpObj(SetOfObj& Alf);
	void SkipSpace();
	bool IsClose();
	bool IsErr(const bool& b);

	void ShowMarshs();
	void ShowObj(SetOfObj& SO);

	void Sort();
	void Find(ListOfObj& TSO, SetOfObj& Alf);
	void Selection(ListOfObj& TSO, ListOfObj& TSOM);

	void MakeMarsh();
};

extern std::fstream FI, FProt;