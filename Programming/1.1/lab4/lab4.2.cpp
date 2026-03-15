/*я—»√ћјя –”“ќ…я—»√ћј
* Задание:
* Даны две последовательности значений a1 ≤ a2 ≤ ... ≤ an и b1 ≤ b2 ≤ ... ≤ bm.
* Сформировать из них последовательность значений с1 ≤ с2 ≤ ... ≤ сn+m.
* Автор: X8X8L				  Версия: 4.1
* Группа: 1488
* Даты: начало: 23.10.2024 - конец: 23.10.2024
*/
#include "iostream"
#include "iomanip"
#include "fstream"

using namespace std;

int main() {
	setlocale(LC_ALL, "RUSSIAN");
	cout.setf(ios::fixed);
	cout.precision(7);
	cout <<
		"Задание:\n" <<
		"\tДаны две последовательности значений a1 <= a2 <= ... <= an и b1 <= b2 <= ... <= bm.\n" <<
		"\tСформировать из них последовательность значений с1 <= с2 <= ... <= сn + m.\n\n" <<
		"Автор: X8X8L				  Версия: 4.2\n" <<
		"Группа: 1488\n" <<
		"Даты: начало: 23.10.2024 - конец: 23.10.2024\n" << endl;

	fstream Fi, Fo("C.txt", ios::out), Ferror("error.txt", ios::out);
	int Real_Size[2]{ 0, 0 };
	double** Array = new double* [2] {nullptr, nullptr};

	for (int i = 0; i < 2; i++) {
		char Name[] = "A1.txt";
		Name[0] += i;
		Fi.open(Name, ios::in);
		if (!Fi.is_open()) {
			cout << "Файл " << Name << " не помещён в корневой каталог программы." << endl;
			Ferror << "Файл " << Name << " не помещён в корневой каталог программы." << endl;
			Fi.close();
			continue;
		}

		double X;
		char C;
		Fi >> C;
		if (!Fi.get(C)) {
			Fi.close();
			cout << "Файл " << Name << " не содержит значений." << endl;
			Ferror << "Файл " << Name << " не содержит значений." << endl;
			continue;
		}
		Fi.clear();
		Fi.seekg(0, ios::beg);

		while (!Fi.eof()) {
			Fi >> X;
			Real_Size[i]++;
		}

		Fi.clear();
		Fi.seekg(0, ios::beg);

		*(Array + i) = new double[Real_Size[i]];

		cout << "\n\tФайл " << char('A' + i) << ".txt: " << endl;

		for (int ii = 0; !Fi.eof(); ii++) {
			Fi >> X;
			*(*(Array + i) + ii) = X;
			cout << *(*(Array + i) + ii) << endl;
		}
		Fi.close();
	}

	int Choose = 0, Iter[2]{ 0,0 };

	cout << "\n\tВыходной файл:" << endl;

	for (; Iter[Choose] < Real_Size[Choose]; Iter[Choose]++) {
		Choose = (Array[0][Iter[0]] < Array[1][Iter[1]]) ? 0 : 1;
		Fo << *(*(Array + Choose) + Iter[Choose]) << endl;
		cout << *(*(Array + Choose) + Iter[Choose]) << endl;
	}

	Choose = (Choose == 1) ? 0 : 1;

	for (; Iter[Choose] < Real_Size[Choose]; Iter[Choose]++) {
		Fo << *(*(Array + Choose) + Iter[Choose]) << endl;
		cout << *(*(Array + Choose) + Iter[Choose]) << endl;
	}

	for (int i = 0; i < 2; i++)
		if (*(Array + i))
			delete[] * (Array + i);
	delete[] Array;

	Ferror.close();
	Fo.close();

	return 0;
}
