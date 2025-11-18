#pragma once

#include <list>
#include <string>
#include <vector>

struct Item {
	std::wstring name;
};

struct FItem : public Item {
	std::vector<Item*> subData;
	~FItem();
};

struct DItem : public Item {
	unsigned short floor;
	unsigned int dups;
};

class Tablica
{
public:
	std::vector<Item*> data;
	~Tablica();
};

