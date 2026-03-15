#pragma once

#include "XSetForm.h"



//struct Marsh;
//struct Object;
//struct Location;
//class SubsForMarsh;
//class ObjectsSet;
//class MarshsSet;
//
//
//struct Location {
//	Object* point, * spos, * time;
//	Location* next = nullptr;
//	Location(Object* point, Object* spos, Object* time);
//};
//
//class SubsForMarsh {
//	Location* start = nullptr, * recur = nullptr, * cur = nullptr;
//public:
//	Location* Add(Location*& Loc);
//	Marsh*& SetCStart();
//	Marsh*& Next();
//	Location& GetC();
//	~SubsForMarsh();
//};
//
//struct Marsh {
//	XSetForm name;
//	Marsh* next = nullptr;
//	SubsForMarsh subs;
//};
//
//
//
//class ObjectsSet {
//	Object* start = nullptr, * recur = nullptr, * cur = nullptr;
//public:
//	Object* Add(XSetForm& XSF);
//	Object*& SetCStart();
//	Object*& Next();
//	Object& GetC();
//	~ObjectsSet();
//};
//
//class MarshsSet {
//	Marsh* start = nullptr, * recur = nullptr, * cur = nullptr;
//public:
//	Marsh* Add(XSetForm& XSF);
//	Marsh*& SetCStart();
//	Marsh*& Next();
//	Marsh& GetC();
//	~MarshsSet();
//};
struct ProtObject {
	//ProtObject();
	XSetForm name;
	//ProtObject* next = nullptr;
	virtual ~ProtObject() = default;
};

struct Conteiner {
	ProtObject* data = nullptr;
	Conteiner* next = nullptr;
	Conteiner(ProtObject* Data);
};

class ProtListOfObj {
protected:
	Conteiner* start = nullptr, * recur = nullptr, * cur = nullptr;
public:
	ProtListOfObj();
	//virtual ListOfObj* clone();
	virtual Conteiner* Add(ProtObject* Obj);
	virtual Conteiner* Find(XSetForm& XSF);
	Conteiner*& SetCStart();
	Conteiner*& Next();
	Conteiner*& GetC();
	virtual ~ProtListOfObj() = default;
};

class ListOfObj : public ProtListOfObj {
public:
	//ListOfObj();
	bool Delete(ProtObject* Obj);
	virtual ~ListOfObj() override;
};



class ListDel : public ListOfObj {
public:
	~ListDel() override;
};

class SetOfObj : public ListDel {
public:
	virtual Conteiner* Add(ProtObject* Obj) override;
	//SetOfObj* clone() override;
	//~SetOfObj() override;
};


class AlfList : public ProtObject, public SetOfObj {
	Conteiner* last = nullptr;
public:
	AlfList(const char& Name);
	Conteiner* Add(ProtObject* Obj) override;
	//Conteiner* Add(Conteiner* Obj);
	Conteiner* Find(XSetForm& XSF) override;
	bool IsNotEnd();
	~AlfList();
};

struct Object : public ProtObject {
	ListOfObj* sub = nullptr;
	Object(XSetForm& XSF, ListOfObj* LO);
	//Object(XSetForm& XSF);
	const bool IsContein(ProtObject* Obj);
	~Object() override;
};

struct Location : public ProtObject {
	//Location();
	Object* point = nullptr, * spos = nullptr, * time = nullptr;
	Location(Object* point, Object* spos, Object* time);
	//Location* next = nullptr;
	~Location() override;
};

//class MarshList : public ListOfObj {
//public:
//	//MarshList();
//	//MarshList* clone() override;
//	virtual ProtObject* Add(ProtObject* Obj) override;
//	~MarshList() override;
//};
//
//struct Marsh : ProtObject{
//	MarshList subs;
//};