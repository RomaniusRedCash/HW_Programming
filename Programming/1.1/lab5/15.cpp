/*я—»√ћјя –”“ќ…я—»√ћј
* Задание:
* Задана квадратная таблица размера N N. Преобразовать ее, осуществив поворот
* элементов вокруг ее центра на 90 по часовой стрелке.
* Автор: X8X8L				  Версия: 4.1
* Группа: 1488
* Даты: начало: 07.11.2024 - конец: --.11.2024
*/
#include "iostream"
#include "fstream"
#include "iomanip"

using namespace std;

fstream Flogs("Logs.txt", ios::out), Fe("Errors.txt", ios::out);

void FReset(fstream& F) {
	F.clear();
	F.seekg(0, ios::beg);
	return;
}
void OutPrint(const char* TXT) {
	cout << TXT << endl;
	Flogs << TXT << endl;
}
void OutPrint(const double& TXT) {
	cout << TXT << ' ';
	Flogs << TXT << ' ';
}

void OutErrors(const char* TXT) {
	cout << TXT << endl;
	Flogs << TXT << endl;
	Fe << TXT << endl;
}

bool IsFile(fstream& F) {

	if (!F.is_open()) {
		OutErrors("Файл не помещён в главную директорию."); //E1
		return false;
	}

	char C;
	if (!F.get(C)) {
		OutErrors("Файл пуст."); //E2
		return false;
	}
	FReset(F);

	double X;
	F >> X;
	if (!(F >> X)) {
		OutErrors("Не указан размер."); //E3
		return false;
	}
	
	return true;
}

int CalcSize(fstream& F) {
	FReset(F);
	int Size = 0;
	F >> Size;

	int RealSX = Size, RealSY = 0;

	char C = '!';

	if (Size <= 0) {
		OutErrors("Указанный размер меньше или равен нулю."); // E4
		Size = 0;
		return Size;
	}

	while (!F.eof()) {
		double X;
		int TempSX = 0;
		while (!F.eof() && C != '\n') {
			F << resetiosflags(ios::skipws);
			F >> C;
			if ((C >= '0' && C <= '9' || C == '-') && TempSX < RealSX) {
				F >> setiosflags(ios::skipws);
				F.seekg(-1, ios::cur);
				F >> X;
				TempSX++;
			}
		}
		C = '!';
		if (TempSX) {
			RealSY++;
			RealSX = TempSX;
			if (RealSX <= RealSY) break;
		}
	}
	if (Size > RealSY) {
		OutErrors("Размер больше реального. Было приравнено к реальному."); //E5
		Size = RealSY;
	}

	F >> setiosflags(ios::skipws);
	return Size;
}

void Inp(double**& A, const int& Size, fstream& F) {
	FReset(F);
	double X;
	char C = '!';
	F >> X;
	int y = 0;
	while (!F.eof()) {
		int x = 0;
		while (!F.eof() && C != '\n') {
			F << resetiosflags(ios::skipws);
			F >> C;
			if ((C >= '0' && C <= '9' || C == '-') && x < Size) {
				F >> setiosflags(ios::skipws);
				F.seekg(-1, ios::cur);
				F >> X;
				A[y][x] = X;
				x++;
			}
		}
		C = '!';
		if (x) y++;
		if (y >= Size) break;
	}
	F >> setiosflags(ios::skipws);

	return;
}

void Out1(double*& A, const int& Size) {
	for (int x = 0; x < Size; x++) {
		OutPrint(A[x]);
	}
	return;
}

void OutA1(double**& A, const int& Size) {
	OutPrint("Ввод:");
	OutPrint("");
	for (int y = 0; y < Size; y++) {
		Out1(A[y], Size);
		OutPrint("");
	}
	return;
}

void Out2(double*& A, const int& Size, fstream& F) {
	for (int x = 0; x < Size; x++) {
		OutPrint(A[x]);
		F << A[x] << ' ';
	}
	return;
}

void OutA2(double**& A, const int& Size, fstream& F) {
	OutPrint("");
	OutPrint("Итог:");
	OutPrint("");
	for (int y = 0; y < Size; y++) {
		Out2(A[y], Size, F);
		OutPrint("");
		F << '\n';
	}
	return;
}

void Procces(double**& A, int& Size) {
	fstream F("FileOut.txt", ios::out);
	int n = Size - 1, n2 = Size / 2 + Size % 2;
	double Temp;
	for (int y = 0; y < n2; y++)
		for (int x = y; x < n - y; x++) {
			Temp = A[y][x];
			A[y][x] = A[n - x][y];
			A[n - x][y] = A[n - y][n - x];
			A[n - y][n - x] = A[x][n - y];
			A[x][n - y] = Temp;
		}
	return;
}

int main() {
	setlocale(LC_ALL, "RUSSIAN");
	cout.precision(7);
	cout.setf(ios::fixed);

	int Size;
	fstream F("File.txt", ios::in);

	if (IsFile(F))
	{
		Size = CalcSize(F);

		double** A = new double* [Size];
		for (int y = 0; y < Size; y++) 
			A[y] = new double[Size];
			
		Inp(A, Size, F);
		F.close();

		F.open("FileOut.txt", ios::out);
		OutA1(A, Size);

		Procces(A, Size);

		OutA2(A, Size, F);

		for (int y = 0; y < Size; y++)
			delete[] A[y];
		delete[] A;
		F.close();
	}

	return 0;
}
