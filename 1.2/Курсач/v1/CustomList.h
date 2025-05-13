#pragma once

template<typename T>
struct Content {
	Content* next = nullptr;
	T data;
};
template<typename T>
class Iter {
	Content<T>* p_it = nullptr;
public:
	Iter(T* Cont);
	bool operator!=(Iter<T>& It);
	Iter<T>& operator++();
	Content<T>& operator*();
	Content<T>* operator*();
};
template<typename T>
class CustomList {
protected:
	Content<T>* start = nullptr, * recur = nullptr, * cur = nullptr, * last = nullptr;
	size_t size = 0;
public:
	Iter<T> begin();
	Iter<T> end();
	virtual T& Add(T Data);
	bool Next();
	//bool NextNew();
	bool SetCStart();
	~CustomList();
};

//////////////////////////////////////////


template<typename T>
Iter<T> CustomList<T>::begin() {
	return Iter<T>(start);
}

template<typename T>
Iter<T> CustomList<T>::end() {
	return Iter<T>(last->next);
}

template<typename T>
T& CustomList<T>::Add(T Data) {
	while (Next());
	Content<T>* Temp = new Content<T>;
	Temp->data = Data;
	recur->next = last = cur = Temp;
	return cur->data;
}

template<typename T>
bool CustomList<T>::Next() {
	recur = cur;
	cur = cur->next;
	return cur;
}

template<typename T>
bool CustomList<T>::SetCStart() {
	recur = start;
	cur = start;
	return cur;
}

template<typename T>
CustomList<T>::~CustomList() {
	for (Content<T>* i : *this)
		delete i;
}

template<typename T>
Iter<T>::Iter(T* Cont) {
	p_it = Cont;
}

template<typename T>
bool Iter<T>::operator!=(Iter<T>& It) {
	return p_it != It.p_it;
}

template<typename T>
Iter<T>& Iter<T>::operator++() {
	p_it = p_it->next;
}

template<typename T>
Content<T>& Iter<T>::operator*() {
	return *p_it;
}

template<typename T>
Content<T>* Iter<T>::operator*() {
	return p_it;
}