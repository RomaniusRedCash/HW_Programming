#pragma once

#include <unordered_map>
#include <string>
#include <iostream>

#define HM std::unordered_map

class IData{
public:
	virtual void printRepeating(std::ostream& os) = 0;
	virtual void printNow(std::ostream& os) = 0;

};

template<typename T1, typename T2>
class DataBase : public IData {
protected:
	typename HM<T1, T2> data;
	typename HM<T1, T2>::iterator cur;
	IData* parent;
public:
	T2& operator[](const T1& i) {return data[i];}
	virtual void printRepeating(std::ostream& os) = 0;
	void printNow(std::ostream& os){
		parent->printNow(os);
		// os <<"PRINT"<<std::endl;
		if (cur == data.end()) cur = data.begin();
		os << cur->first << ' ';
	};
};

class House : public DataBase<int, int> {
public:
	void add(IData* parent, const int& floors);
	void printRepeating(std::ostream& os) override;
};


class Street : public DataBase<int, House> {
public:
	void add(IData* parent, const int& house, const int& floors);
	void printRepeating(std::ostream& os) override;
};

class City : public DataBase<std::string, Street> {
	HM<int, int> floorsMap;
public:
	void add(IData* parent, const std::string& street, const int& house, const int& floors);
	void printRepeating(std::ostream& os) override;
	void printFloors(std::ostream& os) const;
};

class Dater : public DataBase<std::string, City> {
private:
public:
	void add(const std::string& city, const std::string& street, const int& house, const int& floors);
	void printRepeating(std::ostream& os) override;
	void printNow(std::ostream& os) override;
	void printFloors(std::ostream& os) const;
};

