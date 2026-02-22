#pragma once

#include "CustomList.h"


template<typename T>
class CustomSet : public CustomList<T> {
	
public:
	
	//const Content& operator[](const size_t& Id);
	T& Add(T Data) override;
};

////////////////////////////////////////

template<typename T>
T& CustomSet<T>::Add(T Data) {
	Content<T>* Temp = new Content<T>;
	Temp->data = Data;
	if (!this->SetCStart()) {
		this->start = Temp;
		return this->start->data;
	}
	if (this->start->data > Data) {
		this->start = Temp->next = this->start;
		return this->start->data;
	}
	while (this->Next() && Data > this->cur->data);
	if (!this->cur) {
		this->recur->next = this->last = this->cur = Temp;
		return this->cur->data;
	}
	if (this->cur->data > Data) {
		Temp->next = this->cur;
		this->recur->next = this->cur = Temp;
		return this->cur->data;
	}
	delete Temp;
	return this->cur->data;
}
