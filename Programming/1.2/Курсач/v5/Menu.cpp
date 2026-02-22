#include "Menu.h"

const Sost& Menu::GetSost() {	return sost;	}

void Menu::ThrowErr(const char* Err) {
	std::cout << "\n\t" << Err << std::endl;
	throw Err;
}

bool Menu::AnalizIn() {
	GetXSF();

	if (XSF == "help") {
		sost = Help;
	}
	else if (XSF == "exit") {
		sost = Exit;
		return true;
	}
	else if (XSF == "show") {
		std::cout << "\nMar - show all routs\nPoints/Spos/Times - show list of points/transports/times" << std::endl;
		sost = Show;
		return true;
	} 
	else if (XSF == "make") {
		sost = Make;
	}
	else if (XSF == "filter") {
		sost = Select;
	}


	switch (sost) {
	case Help:
		std::cout <<
			"\nhelp - this page" <<
			"\nshow - show content" <<
			"\nfilter - find route" <<
			"\nmake - read you file and analize it" <<
			"\nexit - off" <<
			std::endl;
		return true;
		break;
	case Show:
		if (XSF == "Mar")
			ShowMarshs();
		else if (XSF == "Points")
			ShowObj(AlfPoints);
		else if (XSF == "Spos")
			ShowObj(AlfSpos);
		else if (XSF == "Times")
			ShowObj(AlfTimes);
		return true;
		break;
	case Select:
		Sort();
		sost = Main;
		return true;
		break;
	case Make:
		MakeMarsh();
		sost = Main;
		break;
	case Err:
		break;
	case Exit:
		break;
	default:
		std::cout << "Input is not correct" << std::endl;
		return false;
		break;
	}
}

















bool Menu::Read() {
	while (FI.get(C)) {
		FI.seekg(-1, std::ios::cur);
		Object* MarshObj = GetMarsh();
		IsErr(!MarshObj);
		while (FI.get(C) && C != '<') {
			FI.seekg(-1, std::ios::cur);
			Object* PointObj = GetObjFromF(AlfPoints);
			IsErr(!PointObj || C != '/');
			Object* SposObj = GetObjFromF(AlfSpos);
			IsErr(!SposObj || C != '/');
			Object* TimeObj = GetObjFromF(AlfTimes);
			IsErr(!TimeObj);

			Location* Loc = new Location(PointObj, SposObj, TimeObj);
			MarshObj->sub->Add(Loc);

			PointObj->sub->Add(MarshObj);
			SposObj->sub->Add(MarshObj);
			TimeObj->sub->Add(MarshObj);
		}
		FI.seekg(-1, std::ios::cur);
		IsErr(!IsClose());
		SkipSpace();
	}
	return true;
}

void Menu::ProtAll() {
	if (Marshs.SetCStart()) do {
		Marshs.GetC()->data->name.Prot(FProt);
		Object* Obj = static_cast<Object*>(Marshs.GetC()->data);
		FProt << "\t\\/\\/" << std::endl;
		if (Obj->sub->SetCStart()) do {
			Location* Loc = static_cast<Location*>(Obj->sub->GetC()->data);
			FProt << '\t';
			Loc->point->name.Prot(FProt);
			FProt << '\t';
			Loc->spos->name.Prot(FProt);
			FProt << '\t';
			Loc->time->name.Prot(FProt);
			FProt << "\t\\/\\/\\/" << std::endl;
		} while (Obj->sub->Next());
		FProt << "\tnullptr\n\\/\\/\\/" << std::endl;
	} while (Marshs.Next());
	FProt << "nullptr" << std::endl;

	FProt << "\nPoints:" << std::endl;
	ProtSO(AlfPoints);
	FProt << "\nSpos:" << std::endl;
	ProtSO(AlfSpos);
	FProt << "\nTimes:" << std::endl;
	ProtSO(AlfTimes);
	
}
void Menu::ProtSO(SetOfObj& SO) {
	if (SO.SetCStart()) do {
		Object* AL = static_cast<Object*>(SO.GetC()->data);
		FProt << '\t';
		SO.GetC()->data->name.Prot(FProt);
		FProt << "\t\t\\/\\/" << std::endl;
		if (AL->sub->SetCStart()) do {
			FProt << "\t\t";
			AL->sub->GetC()->data->name.Prot(FProt);
			FProt << "\t\t\\/\\/\\/" << std::endl;
		} while (AL->sub->Next());
		FProt << "\t\tnullptr\n\t\\/\\/\\/" << std::endl;
	} while (SO.Next());
	FProt << "\tnullptr" << std::endl;
}
void Menu::GetXSF() {
	XSF.Clear();
	C = '\0';
	while (std::cin.get(C) && C != '/' && C != '!' && C != '\n')
		XSF.PushEnd(C);
}
Object* Menu::GetMarsh() {
	XSF.Clear();
	SkipSpace();
	FI.get(C);
	if (IsErr(C != '<')) return nullptr;
	while (FI.get(C) && C != '>' && C != '\n')
		XSF.PushEnd(C);
	if (IsErr(C != '>')) return nullptr;
	Object* pMarsh = new Object(XSF, new ListDel);
	return static_cast<Object*>(Marshs.Add(pMarsh)->data);
}

Object* Menu::GetObjFromF(SetOfObj& Alf) {
	XSF.Clear();
	SkipSpace();
	while (FI.get(C) && C != '/' && C != '\n')
		XSF.PushEnd(C);
	return InpObj(Alf);
}

Object* Menu::InpObj(SetOfObj& Alf) {
	Object* Obj = new Object(XSF, new ListOfObj);
	XSetForm AN = XSetForm(XSF[0]);
	Object* AL = new Object(AN, new SetOfObj);
	AL = static_cast<Object*>(Alf.Add(AL)->data);
	return static_cast<Object*>(AL->sub->Add(Obj)->data);
}

void Menu::SkipSpace() {
	while (FI.get(C) && (C == ' ' || C == '\t' || C == '\n'));
	FI.seekg(-1, std::ios::cur);
	FI.clear();
}

bool Menu::IsClose() {
	SkipSpace();
	if (!FI.get(C) || C != '<') return false;
	if (!FI.get(C) || C != '/') return false;
	if (!FI.get(C) || C != '>') return false;
	return true;
}

bool Menu::IsErr(const bool& b) {
	if (b) {
		ThrowErr("File is not succes!");
		return true;
	}
	return false;
}

void Menu::ShowMarshs() {
	size_t IdMar = 0;
	if (Marshs.SetCStart()) do {
		std::cout << IdMar++ << ": " << Marshs.GetC()->data->name << std::endl;
		Object* Obj = static_cast<Object*>(Marshs.GetC()->data);
		size_t Id = 0;
		if (Obj->sub->SetCStart()) do {
			Location* Loc = static_cast<Location*>(Obj->sub->GetC()->data);
			std::cout << '\t' << Id++ << ": " << Loc->point->name << '/';
			std::cout << Loc->spos->name << '/';
			std::cout << Loc->time->name << std::endl;
		} while (Obj->sub->Next());
	} while (Marshs.Next());
}

void Menu::ShowObj(SetOfObj& SO) {
	size_t Id = 0;
	if (SO.SetCStart()) do {
		std::cout << SO.GetC()->data->name << ':' << std::endl;
		Object* AL = static_cast<Object*>(SO.GetC()->data);
		if (AL->sub->SetCStart()) do {
			std::cout << '\t' << Id++ << ": " << AL->sub->GetC()->data->name << std::endl;
		} while (AL->sub->Next());
	} while (SO.Next());
}

void Menu::Sort() {
	ListOfObj TP, TS, TM;
	std::cout << "\nFor separation using '/'" << std::endl; 
	std::cout << "\nSelect points:" << std::endl;
	Find(TP, AlfPoints);
	Selection(TP, TM);
	std::cout << "\nSelect transport:" << std::endl; 
	Find(TS, AlfSpos);
	Selection(TS, TM);

	std::cout << "\nYou can use:" << std::endl;
	if (TM.SetCStart()) do {
		std::cout << TM.GetC()->data->name << std::endl;
	} while (TM.Next()); else std::cout << "\nNot found!\n" << std::endl;
	
}

void Menu::Find(ListOfObj& TSO, SetOfObj& Alf) {
	C = '\0';
	while (C != '\n') {
		GetXSF();
		if (!XSF.GetSize()) return;
		XSetForm AN(XSF[0]);
		Conteiner* Obj = nullptr;
		if (Conteiner* PO = Alf.Find(AN))
			Obj = static_cast<Object*>(PO->data)->sub->Find(XSF);
		if (Obj) TSO.Add(Obj->data);
		else std::cout << "Incorrect data is not using." << std::endl;
	}
}

void Menu::Selection(ListOfObj& TSO, ListOfObj& TSOM) {
	if (!TSOM.SetCStart() && TSO.SetCStart()) {
		Object* T = static_cast<Object*>(TSO.GetC()->data);
		if (T->sub->SetCStart()) do {
			TSOM.Add(T->sub->GetC()->data);
		} while (T->sub->Next());
	}
	if (TSO.SetCStart()) do {
		Object* T = static_cast<Object*>(TSO.GetC()->data);
		if (TSOM.SetCStart()) do {
			if (!T->IsContein(TSOM.GetC()->data))
				TSOM.Delete(TSOM.GetC()->data);
		} while (TSOM.Next());
	} while (TSO.Next());
}

void Menu::MakeMarsh() {
	std::cout << "\nWrite rout's name:" << std::endl;
	GetXSF();
	Object* TM = new Object(XSF, new ListDel);
	if (Marshs.Find(TM->name)) {
		delete TM;
		std::cout << "\nRout already exists!" << std::endl;
		return;
	}
	TM = static_cast<Object*>(Marshs.Add(TM)->data);
	std::cout << "\nWrite - point/transport/time" << std::endl;
	do {
		Object* TP = nullptr, * TS = nullptr, * TT = nullptr;
		GetXSF();
		if (XSF.GetSize() == 0) {
			delete TP;
			delete TS;
			delete TT;
			break;
		}
		TP = InpObj(AlfPoints);
		GetXSF();
		if (XSF.GetSize() == 0) {
			delete TP;
			delete TS;
			delete TT;
			break;
		}
		TS = InpObj(AlfSpos);
		GetXSF();
		if (XSF.GetSize() == 0) {
			delete TP;
			delete TS;
			delete TT;
			break;
		}
		TT= InpObj(AlfTimes);

		Location* Loc = new Location(TP, TS, TT);
		TM->sub->Add(Loc);

		TP->sub->Add(TM);
		TS->sub->Add(TM);
		TT->sub->Add(TM);
	} while (C != '!');
	
	std::fstream FO("Inp.txt", std::ios::app);
	if (TM->sub->SetCStart()) {
		FO << "\n<" << TM->name << '>' << std::endl;
		do {
			Location* Loc = static_cast<Location*>(TM->sub->GetC()->data);
			FO << Loc->point->name << '/';
			FO << Loc->spos->name << '/';
			FO << Loc->time->name << std::endl;
		} while (TM->sub->Next());
		FO << "</>";
		ProtAll();
	}
	FO.close();
}
