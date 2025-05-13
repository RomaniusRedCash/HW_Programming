#include "iostream"
#include "iomanip"
#include "fstream"

using namespace std;

fstream OF("Out.txt", ios::out);

void SmartPrint(const char* C) {
	OF << C;
}

struct CustomStringKal {
protected:
	char* str = nullptr;
	char Mark;
	void CreateStr(size_t size, const char& Mark) {
		this->Mark = Mark;
		str = new char[size + 1];
		str[size] = Mark;
	}
public:
	CustomStringKal(size_t size, const char& Mark) {
		CreateStr(size, Mark);
	}
	CustomStringKal(const CustomStringKal& Str) : Mark(Str.Mark) {
		CreateStr(Str.GetSize(), Str.Mark);
		for (int i = 0; Str[i] != Mark; i++) str[i] = Str[i];
	}
	char& operator[](size_t i) {
		return str[i];
	}
	const char& operator[](size_t i) const {
		return str[i];
	}
	friend ostream& operator<<(ostream& OF, CustomStringKal& Str) {
		for (char& i : Str) {
			OF << i;
		}
		return OF;
	}
	char* begin() {
		return &str[0];
	}
	char* end() {
		for (int i = 0; ; i++)
			if (str[i] == Mark)
			{
				return &str[i];
			}
	}
	const size_t GetSize() const {
		size_t T = 0;
		for (; str[T] != Mark; T++);
		return T;
	}
	~CustomStringKal() {
		delete[] str;
	}
};

CustomStringKal Inp1(fstream F) {
	if (F.is_open()) {
		char c, Mark = '\0', Point = '\n';
		for (int i = 0; i < 2 && F.get(c); i++) {
			if (i) {
				Mark = c;
				break;
			} else
				Point = c;
			if (c == '\n')	i--;
		}
		streampos TempPos = F.tellg();
		F.get(c);
		if (c == '\n') TempPos = F.tellg();
		else F.seekg(TempPos);
		size_t size_str = 0;
		while (F.get(c) && c != '\n' && c != Point && c != Mark)
			size_str++;
		F.clear();
		F.seekg(TempPos);
		CustomStringKal Str(size_str, Mark);
		for (int i = 0; F.get(c) && c != '\n' && c != Mark && c != Point; i++)
			Str[i] = c;
		return Str;
	}
	else
		SmartPrint("File is not open!\n");
	return CustomStringKal(0, '\0');
}

void OutStr(const char* Cr, CustomStringKal& Str) {
	OF << Cr << Str << endl;
}

void Procces(CustomStringKal& Str) {
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
	}
	if (b) {
		if (Temp == 0)
			SmartPrint("Vse skoby zakrity!\n");
		if (Temp > 0) {
			SmartPrint("Iest ne zakritie skoby!\n");
		}
	}
	else
		SmartPrint("Net Skob!!!\n");
}

int main() {
	setlocale(LC_ALL, "russian");
	CustomStringKal Str = Inp1(fstream("Inp.txt", ios::in));
	OutStr("Считана строка: ", Str);
	Procces(Str);

	return 0;
}
