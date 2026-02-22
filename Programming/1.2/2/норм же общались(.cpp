#include "iostream"
#include "iomanip"
#include "fstream"

using namespace std;

void SmartPrint(const char* C) {
	cout << C;
}

struct CustomStringKal {
protected:
	char* str = nullptr;
	const char Mark = '\0';
	size_t size_str = 0;
	void PrintLOG() {
		cout << "str whith len " << size_str << endl;
	}
	void SubConstr(const char* c) {
		for (size_str = 0; c[size_str] != Mark; size_str++);
		CreateStr();
		for (size_str = 0; c[size_str] != Mark; size_str++) {
			str[size_str] = c[size_str];
		}
	}
	void SubDel() {
		if (str) {
			delete[] str;
			size_str = 0;
		}
	}
	void CreateStr() {
		str = new char[size_str+1];
		str[size_str] = Mark; 
		//PrintLOG();
	}
	//struct Iter {
	//	char* iter_c;
	//	Iter(char* str, size_t pos) {
	//		iter_c = &str[pos];
	//	}
	//	Iter& operator++() {
	//		iter_c++;
	//		return *this;
	//	}
	//	char& operator*() {
	//		return *iter_c;
	//	}
	//	//bool operator==(Iter& it2) {
	//	//	return iter_c == it2.iter_c;
	//	//}
	//	bool operator!=(const Iter& it2) {
	//		return iter_c != it2.iter_c;
	//	}
	//};
public:
	CustomStringKal(size_t size) : size_str(size+1) {
		CreateStr();
	};
	CustomStringKal(const char* c) {
		SubConstr(c);
	}
	CustomStringKal(fstream F) {
		if (F.is_open()) {
			SubDel();
			char c;
			while (F.get(c))
				size_str++;
			if (size_str == 0) SmartPrint("File is empty!\n");
			F.clear();
			F.seekg(0, ios::beg);
			CreateStr();
			for (size_str = 0; F.get(c); size_str++) {
				str[size_str] = c;
			}
		}
		else
			SmartPrint("File is not open!\n");
	}
	CustomStringKal& operator=(const char* str2) {
		SubConstr(str2);
		return *this;
	}
	//CustomStringKal& GetStr_from_file(fstream& F) {
	//	
	//}
	char& operator[](size_t i) {
		return str[i];
	}
	//Iter begin() {
	//	return Iter(str, 0);
	//}
	//Iter end() {
	//	return Iter(str, size_str);
	//}
	char* begin() {
		return &str[0];
	}
	char* end() {
		return &str[size_str];
	}
	~CustomStringKal() {
		SubDel();
	}
	void PrintAsLog() {
		cout << str << endl;
	}
};

int main() {
	CustomStringKal Str(fstream("Inp.txt", ios::in));
	//CustomStringKal Str = "((()))";
	long long Temp = 0;
	bool b = false;
	for (char& c : Str) {
		switch (c)
		{
		case '(':
			Temp++;
			b = true;
			break;
		case ')':
			Temp--;
			b = true;
			break;
		default:
			break;
		}
		if (Temp < 0) {
			SmartPrint("Suschestvuiet ne otkritaya skoba!\n");
			break;
		}
		//cout << c << endl;
	}
	if (b) {
		if (Temp == 0)
			SmartPrint("Vse skoby zakrity!\n");
		if (Temp > 0) {
			SmartPrint("Iest ne zakritie skoby!\n");
		}
	} else
		SmartPrint("Net Skob!!!\n");

	return 0;
}
