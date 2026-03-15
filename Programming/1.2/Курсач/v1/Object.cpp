#include "Object.h"

template<typename T>
XSetForm& Object<T>::GetName() {
	return name;
}

template<typename T>
CustomList<T>& Object<T>::GetSubs() {
	return sub_objects;
}

template<typename T>
Object<T>::Object(XSetForm& Name) : name(Name) {
}

template<typename T>
bool Object<T>::operator>(Object<T>& Obj) {
	return name > Obj.name;
}
template<typename T>
bool Object<T>::AddInto(T Obj) {
	return sub_objects.Add(Obj);
}

//Marsh::Marsh(XSetForm& Name) : name(Name) {
//}
//
//bool Marsh::operator>(Marsh& Obj) {
//	return name > Obj.name;
//}
