#include "Object.h"

extern std::fstream FI, FProt;

Conteiner* ListOfObj::Add(ProtObject* Obj) {
	Conteiner* TD = new Conteiner(Obj);
	if (!SetCStart()) {
		start = TD;
		return SetCStart();
	}
	while (Next());
	recur->next = TD;
	return cur = TD;
}

Conteiner* ListOfObj::Find(XSetForm& XSF) {
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
	while (Next())
		if (cur->data == Obj) {
			recur->next = cur->next;
			delete cur;
			cur = recur->next;
			return true;
		}
	return false;
}

Conteiner*& ListOfObj::SetCStart() { return cur = recur = start; }

Conteiner*& ListOfObj::Next() {
	recur = cur;
	if (!cur) return cur;
	return cur = cur->next;
}

Conteiner*& ListOfObj::GetC() {	return cur; }

ListOfObj::~ListOfObj() {
	SetCStart();
	Next();
	while (recur) {
		delete recur;
		Next();
	}
}

Conteiner* SetOfObj::Add(ProtObject* Obj) {
	Conteiner* TD = new Conteiner(Obj);
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

Object::Object(XSetForm& XSF, ListOfObj* LO) {
	name = XSF;
	sub = LO;
}
const bool Object::IsContein(ProtObject* Obj) {
	return sub->Find(Obj->name);
}

Object::~Object() {	if (sub) delete sub; }

Location::Location(Object* point, Object* spos, Object* time) : point(point), spos(spos), time(time) {}

ListDel::~ListDel() {
	SetCStart();
	Next();
	while (recur) {
		delete recur->data;
		Next();
	}
}

Conteiner::Conteiner(ProtObject* Data) : data(Data) {}