#pragma once

#include "CustomSet.h"
#include "XSetForm.h"




//class Marsh {
//	CustomList<PosM*> pos;
//	XSetForm name;
//public:
//	Marsh(XSetForm& Name);
//	bool operator>(Marsh& Obj);
//};
template<typename T>
class Object {
	CustomList<T> sub_objects;
	XSetForm name;
public:
	XSetForm& GetName();
	CustomList<T>& GetSubs();
	Object(XSetForm& Name);
	bool operator>(Object<T>& Obj);
	bool AddInto(T MarshObj);
};

struct PosM {
	Object<Object<PosM>*>* point, * spos, * time;
};