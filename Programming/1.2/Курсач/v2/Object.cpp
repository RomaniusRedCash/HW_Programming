#include "Object.h"

//#include "Global.cpp"
//size_t NEW = 0;
//
//Location*& SubsForMarsh::Add(Location*& Loc) {
//	recur = cur;
//	if (!start) return cur = start = Loc;
//	return cur = Loc;
//}
//
//const bool SubsForMarsh::SetCStart() {
//	return recur = cur = start;
//}
//
//const bool SubsForMarsh::Next() {
//	recur = cur;
//	if (!cur) return false;
//	return cur = cur->next;
//}
//
//Location& SubsForMarsh::GetC() {
//	return *cur;
//}
//
//SubsForMarsh::~SubsForMarsh() {
//	if(!SetCStart()) return;
//	Next();
//	while (recur) {
//		delete recur; 
//		Next();
//	}
//}
//
//Object* ObjectsSet::Add(XSetForm& XSF) {
//	Object* Obj = new Object;
//	Obj->name = XSF;
//	if (!SetCStart()) {
//		start = Obj;
//		return SetCStart();
//	}
//	if (Obj->name < start->name) {
//		Obj->next = start;
//		return start = Obj;
//	}
//	while (Next() && cur->name < Obj->name);
//	if (!cur) {
//		recur->next = Obj;
//		return cur = Obj;
//	}
//	if (Obj->name < cur->name) {
//		recur->next = Obj;
//		Obj->next = cur;
//		return recur = Obj;
//	}
//	delete Obj;
//	return nullptr;
//}
//
//Object*& ObjectsSet::SetCStart() {
//	return recur = cur = start;
//}
//
//Object*& ObjectsSet::Next() {
//	recur = cur;
//	if (!cur) return cur;
//	return cur = cur->next;
//}
//
//Object& ObjectsSet::GetC() {
//	return *cur;
//}
//
//ObjectsSet::~ObjectsSet() {
//	if (!SetCStart()) return;
//	Next();
//	while (recur) {
//		delete recur;
//		Next();
//	}
//}
//
//Marsh* MarshsSet::Add(XSetForm& XSF) {
//	Marsh* Obj = new Marsh;
//	Obj->name = XSF;
//	if (!SetCStart()) {
//		start = Obj;
//		return SetCStart();
//	}
//	if (Obj->name < start->name) {
//		Obj->next = start;
//		return start = Obj;
//	}
//	while (Next() && cur->name < Obj->name);
//	if (!cur) {
//		recur->next = Obj;
//		return cur = Obj;
//	}
//	if (Obj->name < cur->name) {
//		recur->next = Obj;
//		Obj->next = cur;
//		return recur = Obj;
//	}
//	delete Obj;
//	return nullptr;
//}
//
//Marsh*& MarshsSet::SetCStart() {
//	return cur = recur = start;
//}
//
//Marsh*& MarshsSet::Next() {
//	recur = cur;
//	if (!cur) return cur;
//	return cur = cur->next;
//}
//
//Marsh& MarshsSet::GetC() {
//	return *cur;
//}
//
//MarshsSet::~MarshsSet() {
//	if (!SetCStart()) return;
//	Next();
//	while (recur) {
//		delete recur;
//		Next();
//	}
//}
//
//Location::Location(Object* point, Object* spos, Object* time) : point(point), spos(spos), time(time) {
//}


extern std::fstream FI, FProt;


ProtListOfObj::ProtListOfObj() {
	//std::cout << NEW++ << "NewLPO" << NEW << std::endl;
	FProt << "init " << this << std::endl;
}

Conteiner* ProtListOfObj::Add(ProtObject* Obj) {
	FProt << "To " << this << " add " << Obj << " " << Obj->name << std::endl;
	Conteiner* TD = new Conteiner(Obj);
	if (!SetCStart()) {
		start = TD;
		return SetCStart();
	}
	while (Next());
	recur->next = TD;
	return cur = TD;
}

Conteiner* ProtListOfObj::Find(XSetForm& XSF) {
	if (SetCStart()) do {
		if (cur->data->name == XSF)
			return cur;
	} while (Next());
	return nullptr;
}

bool ListOfObj::Delete(ProtObject* Obj) {
	if (!SetCStart()) return false;
	if (start->data == Obj) {
		Next();
		delete start;
		start = cur;
		SetCStart();
		return true;
	}
	while (Next()) {
		if (cur->data == Obj) {
			recur->next = cur->next;
			delete cur;
			cur = recur->next;
			return true;
		}
	}
	return false;
}

Conteiner*& ProtListOfObj::SetCStart() {
	return cur = recur = start;
}

Conteiner*& ProtListOfObj::Next() {
	recur = cur;
	if (!cur) return cur;
	return cur = cur->next;
}

Conteiner*& ProtListOfObj::GetC() {
	return cur;
}

ListOfObj::~ListOfObj() {
	SetCStart();
	Next();
	while (recur) {
		FProt << recur << std::endl;
		delete recur;
		Next();
	}
	////std::cout << NEW-- << "DelL" << NEW << std::endl;
	//std::cout << "delete " << this << std::endl;
	//SetCStart();
	//Next();
	//while (recur) {
	//	std::cout << "\t\t\tDel " << recur << std::endl;
	//	delete recur;
	//	Next();
	//}
}

//MarshList* MarshList::clone() {
//	return new MarshList(*this);
//}
//
//MarshList::MarshList() {
//	//std::cout << NEW++ << "NewML" << NEW << std::endl;
//}
//
//ProtObject* MarshList::Add(ProtObject* Obj) {
//	std::cout << "To " << this << " add " <<
//		static_cast<Location*>(Obj)->point->name << ' ' <<
//		static_cast<Location*>(Obj)->point->name << ' ' <<
//		static_cast<Location*>(Obj)->point->name << ' ' << Obj << std::endl;
//	return ListOfObj::Add(Obj);
//}
//
//ProtObject* SetOfObj::Add(ProtObject* Obj) {
//	return ListOfObj::Add(Obj);
//}
//
//SetOfObj* SetOfObj::clone() {
//	return new SetOfObj(*this);
//}

Conteiner* SetOfObj::Add(ProtObject* Obj) {
	Conteiner* TD = new Conteiner(Obj);
	FProt << "To " << this << " add " << TD << " Con " << TD->data->name << std::endl;
	if (!SetCStart()) {
		start = TD;
		return SetCStart();
	}
	if (Obj->name < start->data->name) {
		TD->next = start;
		start = TD;
		return SetCStart();
	}
	while (cur->data->name < Obj->name && Next());
	if (!cur) {
		recur->next = TD;
		return cur = TD;
	}
	if (Obj->name < cur->data->name) {
		recur->next = TD;
		TD->next = cur;
		return cur = TD;
	}
	delete TD;
	delete Obj;
	return cur;
}
//
//SetOfObj::~SetOfObj() {
//	//if (start)
//	//	std::cout << start->name << std::endl;
//	////std::cout << NEW-- << "DelLO" << NEW << std::endl;
//	//SetCStart();
//	//if (Next()) do {
//	//	delete static_cast<Object*>(recur);
//	//} while (Next());
//}
//
//MarshList::~MarshList() {
//	if (start)
//		std::cout << start->name << std::endl;
//	////std::cout << NEW-- << "DelLM" << NEW << std::endl;
//	//SetCStart();
//	//if (Next()) do {
//	//	delete static_cast<Location*>(recur);
//	//} while (Next());
//}
//
//Location::Location(Object* point, Object* spos, Object* time) : point(point), spos(spos), time(time) {
//}
//
Object::Object(XSetForm& XSF, ListOfObj* LO) {
	name = XSF;
	sub = LO;
	FProt << "init " << this << ' ' << name << std::endl;
}
const bool Object::IsContein(ProtObject* Obj) {
	return sub->Find(Obj->name);
}
//Object::Object(XSetForm& XSF) {
//	sub = new ListOfObj;
//	name = XSF;
//	std::cout << "init " << this << ' ' << name << std::endl;
//}
//
Object::~Object() {
	FProt << "Delete " << this << ' ' << name << std::endl;
	if (sub) delete sub;
}
//ProtObject::ProtObject() {
//	//std::cout << NEW++ << "NewPO" << NEW << std::endl;
//}
//
//ProtObject::~ProtObject() {
//	//std::cout << NEW-- << "DelPO" << NEW << std::endl;
//}
//
Location::Location(Object* point, Object* spos, Object* time) : point(point), spos(spos), time(time) {
	//std::cout << NEW++ << "NewL" << NEW << std::endl;
	FProt << "Init " << this << ' ' <<
		point->name <<
		spos->name <<
		time->name << std::endl;
}

Location::~Location() {
	//std::cout << NEW-- << "DelL" << NEW << std::endl;
	FProt << "Delete " << this << ' '
		<< point->name << ' '
		<< spos->name << ' '
		<< time->name << std::endl;
}

ListDel::~ListDel() {
	FProt << "delete " << this << std::endl;
	SetCStart();
	Next();
	while (recur) {
		//std::cout << "\t\t\tDel " << recur << std::endl;
		delete recur->data;
		Next();
	}
}

AlfList::AlfList(const char& Name) {
	FProt << "Init " << this << std::endl;
	name.PushEnd(Name);
}

Conteiner* AlfList::Add(Conteiner* Obj) {
	FProt << "To " << this << ' ' << name << " add " << Obj << " " << Obj->data->name << std::endl;
	if (Obj->data->name[0] != name[0]) return nullptr;
	if (!start) return start = last = Obj;
	if (Obj->data->name < start->data->name) return start = Obj;
	if (last->data->name < Obj->data->name) return last = Obj;
	return nullptr;
}

Conteiner* AlfList::Find(XSetForm& XSF) {
	if (XSF[0] != name[0]) return nullptr;
	if (SetCStart()) do {
		if (cur->data->name == XSF) return cur;
	} while (Next() && IsNotEnd());
	return nullptr;
}

bool AlfList::IsNotEnd() {
	return cur != last->next;
}

Conteiner::Conteiner(ProtObject* Data) : data(Data) {
	FProt << "Init " << this << " with " << Data << std::endl;
}
