#include <iostream>

void PrintBit(const unsigned char& Data) {
	std::cout << Data << ": ";
	unsigned char Mask = 1;
	Mask <<= sizeof(Data) * 8 - 1;
	while (Mask) {
		std::cout << bool(Data & Mask);
		Mask >>= 1;
	}
	std::cout << std::endl;
}

void PrintBit(const double& Data) {
	std::cout << Data << ": ";
	size_t Mask = 1;
	size_t* Sub = (size_t*)&Data;
	Mask <<= sizeof(Data) * 8 - 1;
	while (Mask) {
		std::cout << bool(*Sub & Mask);
		Mask >>= 1;
	}
	std::cout << std::endl;
}

template<typename T>
void InPar(const T& Data, size_t& Raz, size_t& Kol) {
	Raz = 0; Kol = 0;
	std::cout << "Разрядов: " << sizeof(Data) * 8 << std::endl;
	std::cout << "Введите старший разряд и колличество для сдвига:" << std::endl;
	while (!(std::cin >> Raz >> Kol) || Raz > sizeof(Data) * 8 || Raz < 2 || Kol < 2 || Raz < Kol) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "Данные не коректны, попробуйте ещё раз:" << std::endl;
	}
}

void Remake(unsigned char& Data) {
	size_t Raz, Kol;
	InPar(Data, Raz, Kol);
	PrintBit(Data);
	unsigned char Mask = 1;
	size_t Zeros = Kol;
	Mask <<= Raz - 1;
	for (size_t i = 0; i < Kol; i++) {
		Zeros -= bool(Data & Mask);
		Data &= ~Mask;
		Mask >>= 1;
	}
	Mask = 1 << Raz - Zeros - 1;
	for (size_t i = 0; i < Kol - Zeros; i++) {
		Data += Mask;
		Mask >>= 1;
	}
	PrintBit(Data);
}

void Remake(double& Data) {
	size_t Raz, Kol;
	InPar(Data, Raz, Kol);
	PrintBit(Data);
	size_t Mask = 1;
	size_t* Sub = (size_t*)&Data;
	size_t Zeros = Kol;
	Mask <<= Raz - 1;
	for (size_t i = 0; i < Kol; i++) {
		Zeros -= bool(*Sub & Mask);
		*Sub &= ~Mask;
		Mask >>= 1;
	}
	Mask = size_t(1) << Raz - Zeros - 1;
	for (size_t i = 0; i < Kol - Zeros; i++) {
		*Sub += Mask;
		Mask >>= 1;
	}
	PrintBit(Data);
}

int main() {
	setlocale(LC_ALL, "RUSSIAN");
	unsigned char C;
	std::cout << "Введите данные типа unsigned char:\t" << std::endl;
	std::cin >> C;
	Remake(C);
	double D;
	std::cout << "Введите данные типа double:\t" << std::endl;
	std::cin >> D;
	Remake(D);
	return 0;
}