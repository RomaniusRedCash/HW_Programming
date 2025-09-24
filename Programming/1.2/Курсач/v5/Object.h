#pragma once

#include "XSetForm.h"

struct ProtObject {
	XSetForm name;
	virtual ~ProtObject() = default;
};

struct Conteiner {
	ProtObject* data = nullptr;
	Conteiner* next = nullptr;
	Conteiner(ProtObject* Data);
};

class ListOfObj {
protected:
	Conteiner* start = nullptr, * recur = nullptr, * cur = nullptr;
public:
	virtual Conteiner* Add(ProtObject* Obj);
	virtual Conteiner* Find(XSetForm& XSF);
	Conteiner*& SetCStart();
	Conteiner*& Next();
	Conteiner*& GetC();
	bool Delete(ProtObject* Obj);
	virtual ~ListOfObj();
};

class ListDel : public ListOfObj {
public:
	~ListDel() override;
};

class SetOfObj : public ListDel {
public:
	virtual Conteiner* Add(ProtObject* Obj) override;
};

struct Object : public ProtObject {
	ListOfObj* sub = nullptr;
	Object(XSetForm& XSF, ListOfObj* LO);
	const bool IsContein(ProtObject* Obj);
	~Object() override;
};

struct Location : public ProtObject {
	Object* point = nullptr, * spos = nullptr, * time = nullptr;
	Location(Object* point, Object* spos, Object* time);
};