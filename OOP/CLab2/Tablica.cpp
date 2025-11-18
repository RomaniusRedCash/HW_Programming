#include "Tablica.h"

Tablica::~Tablica()
{
	for (const Item* i : data)
		delete i;
}

FItem::~FItem()
{
	for (const Item* i : subData)
		delete i;
}
