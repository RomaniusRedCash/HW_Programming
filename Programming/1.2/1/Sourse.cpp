#include "iostream"
#include "iomanip"
#include "fstream"

using namespace std;
fstream OF("Out.txt", ios::out);
void SmartPrint(const char* C) {OF << C;}
class STR_N {
	size_t size_str = 0;
	char* str = nullptr;
	bool Good(fstream& F);
	void Del();
	void SetSTR(fstream& F);
public:
	void inp1(fstream F);
	void inp2(fstream F);
	~STR_N();
	char* begin();
	char* end();
};
char* STR_N::begin() {return str;}
char* STR_N::end() {return &str[size_str];}
bool STR_N::Good(fstream& F) {
	if (!F.is_open() || !F.get()) {
		SmartPrint("Файл не существует или пуст!\n");
		return false;
	}
	F.seekg(0, ios::beg);
	return true;
}
void STR_N::Del() {
	size_str = 0;
	delete[] str;
}
void STR_N::SetSTR(fstream& F) {
	if (!str) {
		str = new char[size_str];
		for (char& c : *this)
			F.get(c);
	}
}
void STR_N::inp1(fstream F) {
	if (!Good(F)) return;
	Del();
	F << setiosflags(ios::skipws);
	char c, Point = '\n';
	while (F.get(Point) && Point == '\n');
	streampos TempPos = F.tellg();
	F.get(c);
	if (c == '\n')
		TempPos = F.tellg();
	else
		F.seekg(TempPos);
	while (F.get(c) && c != '\n' && c != Point)
		size_str++;
	F.clear(); F.seekg(TempPos);
	SetSTR(F);
}
void STR_N::inp2(fstream F) {
	if (!Good(F)) return;
	Del();
	F << setiosflags(ios::skipws);
	char c;
	size_t Size = 0;
	while (F.get(c)) {
		if (c <= '9' && c >= '0') {
			F.seekg(-1, ios::cur);
			F >> Size;
			break;
		}
	}
	if (!Size) {
		SmartPrint("Размер не указан или равен 0!\n");
		return;
	}
	streampos TempPos = F.tellg();
	F.get(c);
	if (c == '\n')
		TempPos = F.tellg();
	else
		F.seekg(TempPos);
	while (F.get(c) && c != '\n' && size_str < Size)
		size_str++;
	F.clear(); F.seekg(TempPos);
	SetSTR(F);
}
STR_N::~STR_N() {
	Del();
}
std::ostream& operator << (std::ostream& OF, STR_N& Str) {
	for (char& c : Str) OF << c;
	return OF;
}
//////////////////////////////////////////////////////////////////////////////////////////
struct CustomStringKal {
	char* str = nullptr;
	char mark = '\0';
	void SetSize(const size_t& Size) {
		if (!str) {
			str = new char[Size + 1];
			str[Size] = mark;
		}
	}
	char& operator[](const size_t& i) {return str[i];}
	~CustomStringKal() {delete[] str;}
};
ostream& operator<<(ostream& OF, CustomStringKal& Str) {
	for (int i = 0; Str[i] != Str.mark; i++)
		OF << Str[i];
	return OF;
}
CustomStringKal Inp1(fstream F) {
	CustomStringKal Str;
	if (F.is_open() && F.get()) {
		F.seekg(0, ios::beg);
		char c, Point = '\n';
		for (short i = 0; i < 2 && F.get(c); i++) {
			if (c != '\n') {
				if (i) {
					Point = c;
				}
				else {
					Str.mark = c;
				}
			}
			else i--;
		}
		streampos TempPos = F.tellg();
		F.get(c);
		if (c == '\n')
			TempPos = F.tellg();
		else
			F.seekg(TempPos);
		size_t size_str = 0;
		while (F.get(c) && c != '\n' && c != Point && c != Str.mark)
			size_str++;
		Str.SetSize(size_str);
		F.clear();
		F.seekg(TempPos);
		for (int i = 0; i < size_str && F.get(c); i++)
			Str[i] = c;
	}
	else
		SmartPrint("Файл не существует или пуст!\n");
	return Str;
}

CustomStringKal Inp2(fstream F) {
	CustomStringKal Str;
	F << setiosflags(ios::skipws);
	if (F.is_open() && F.get()) {
		F.seekg(0, ios::beg);
		char c;
		size_t Size = 0;
		for (short i = 0; i < 2 && F.get(c); i++) {
			if (c != '\n') {
				if (i) {
					F.seekg(-1, ios::cur);
					F >> Size;
				}
				else
					Str.mark = c;
			}
			else i--;
		}
		streampos TempPos = F.tellg();
		F.get(c);
		if (c == '\n')
			TempPos = F.tellg();
		else
			F.seekg(TempPos);
		size_t size_str = 0;
		while (F.get(c) && c != '\n' && size_str < Size && c != Str.mark)
			size_str++;
		Str.SetSize(size_str);
		F.clear();
		F.seekg(TempPos);
		for (int i = 0; i < size_str && F.get(c); i++)
			Str[i] = c;
	}
	else
		SmartPrint("Файл не существует или пуст!\n");
	return Str;
}
void OutStr(const char* Cr, CustomStringKal& Str) {OF << Cr << Str << endl;}
void OutStr(const char* Cr, STR_N& Str) {OF << Cr << Str << endl;}
void Procces(CustomStringKal& Str) {
	long long Temp = 0;
	bool b = false;
	for (size_t i = 0; Str[i] != Str.mark; i++) {
		switch (Str[i])
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
			SmartPrint("Есть не открытая скобка!\n");
			break;
		}
	}
	if (b) {
		if (Temp == 0)
			SmartPrint("Все скобки закрыты.\n");
		if (Temp > 0)
			SmartPrint("Есть не закрытая скобка!\n");
	}
	else
		SmartPrint("Скобок в строке нет!\n");
}
void Procces(STR_N& Str) {
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
			SmartPrint("Есть не открытая скобка!\n");
			break;
		}
	}
	if (b) {
		if (Temp == 0) SmartPrint("Все скобки закрыты.\n");
		if (Temp > 0) SmartPrint("Есть не закрытая скобка!\n");
	} else
		SmartPrint("Скобок в строке нет!\n");
}
int main() {
	//CustomStringKal Str = Inp2(fstream("Inp.txt", ios::in));
	STR_N Str;
	Str.inp2(fstream("Inp.txt", ios::in));
	OutStr("Считана строка: ", Str);
	Procces(Str);
	return 0;
}