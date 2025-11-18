#pragma once

#include <unordered_map>
#include <string>

#define HM std::unordered_map

struct HomeStr {
	int floors, repeats = 0;
};

struct City {
	HM<std::wstring, HM<int, HomeStr>> subData;
	HM<int, int> floorsMap;
	HM<int, HomeStr> operator[] (const std::wstring& city);
};

class Dater {
public:
	HM<std::wstring, City> data;

	City operator[] (const std::wstring& city);
	void add(const std::wstring& city, const std::wstring& street, const int& house, const int& floors);
};

