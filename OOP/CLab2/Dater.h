#pragma once

#include <unordered_map>
#include <string>
#include <iostream>

#define HM std::unordered_map

template<typename T1, typename T2>
class DataBase {
protected:
	HM<T1, T2> data;
public:
	virtual std::vector<T2> printRepeating(const T1& i) = 0;
};

class house : DataBase<int, int> {
public:
	void add(const int& floors);
	std::vector<int> printRepeating(const int& i) override;
};

class Street : DataBase<int, house> {
public:
	void add(const int& house, const int& floors);
};

class City : DataBase<std::wstring, Street> {
	HM<int, int> floorsMap;
public:
	//HM<int, house> operator[] (const std::wstring& city);
	void add(const std::wstring& street, const int& house, const int& floors);
};

class Dater : DataBase<std::wstring, City> {
private:
public:
	//City operator[] (const std::wstring& city);
	void add(const std::wstring& city, const std::wstring& street, const int& house, const int& floors);
};

