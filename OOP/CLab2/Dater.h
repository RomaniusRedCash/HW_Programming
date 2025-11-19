#pragma once

#include <unordered_map>
#include <string>
#include <iostream>

#define HM std::unordered_map

class IData{
public:
	virtual void printRepeating(std::wostream& os) = 0;
	virtual void printNow(std::wostream& os) = 0;

};

template<typename T1, typename T2>
class DataBase : public IData {
protected:
	typename HM<T1, T2> data;
	typename HM<T1, T2>::iterator cur;
	IData* parent;
public:
	T2& operator[](const T1& i) {return data[i];}
	virtual void printRepeating(std::wostream& os) = 0;
	void printNow(std::wostream& os){
		parent->printNow(os);
		// os <<L"PRINT"<<std::endl;
		if (cur == data.end()) cur = data.begin();
		os << cur->first << L' ';
	};
};

class house : public DataBase<int, int> {
public:
	void add(IData* parent, const int& floors);
	void printRepeating(std::wostream& os) override;
};


class Street : public DataBase<int, house> {
public:
	void add(IData* parent, const int& house, const int& floors);
	void printRepeating(std::wostream& os) override;
};

class City : public DataBase<std::wstring, Street> {
	HM<int, int> floorsMap;
public:
	void add(IData* parent, const std::wstring& street, const int& house, const int& floors);
	void printRepeating(std::wostream& os) override;
	void printFloors(std::wostream& os) const;
};

class Dater : public DataBase<std::wstring, City> {
private:
public:
	void add(const std::wstring& city, const std::wstring& street, const int& house, const int& floors);
	void printRepeating(std::wostream& os) override;
	void printNow(std::wostream& os) override;
	void printFloors(std::wostream& os) const;
};

