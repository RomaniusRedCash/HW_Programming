/*я—»√ћјя –”“ќ…я—»√ћј
* Задание:
* Даны две последовательности значений a1 ≤ a2 ≤ ... ≤ an и b1 ≤ b2 ≤ ... ≤ bm.
* Сформировать из них последовательность значений с1 ≤ с2 ≤ ... ≤ сn+m.
* Автор:X8X8L				  Версия: 4.1
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
		"Автор:X8X8L				  Версия: 4.1\n" <<
		"Группа: 1488\n" <<
		"Даты: начало: 23.10.2024 - конец: 23.10.2024\n" << endl;

	fstream Fi, Fo("C.txt", ios::out), Ferror("error.txt", ios::out);
	int Size[2]{ 0, 0 }, Real_Size[2]{ 0, 0 };
	double Array[2][10000];

	for (int i = 0; i < 2; i++) {
		char Name[] = "A.txt";
		Name[0] += i;
		Fi.open(Name, ios::in);
		if (!Fi.is_open()) {
			cout << "Файл " << Name << " не помещён в корневой каталог программы." << endl;
			Ferror << "Файл " << Name << " не помещён в корневой каталог программы." << endl;
			Fi.close();
			continue;
		}

		double X;

		while (!Fi.eof()) {
			Fi >> X;
			Real_Size[i]++;
			if (Real_Size[i] > 10000) {
				cout << "Длина файла больше 10000, что дальше эьтого значения - учитываться ен будет!" << endl;
				Ferror << "Длина файла больше 10000, что дальше эьтого значения - учитываться ен будет!" << endl;
			}
		}

		Fi.clear();
		Fi.seekg(0, ios::beg);
		Fi >> Size[i];
		if (Size[i] < 0)
		{
			cout << "Значение чисел отрицательно, было приравнено к нулю" << endl;
			Ferror << "Значение чисел отрицательно, было приравнено к нулю" << endl;
			Size[i] = 0;
			Fi.close();
			continue;
		}
		if (Fi.eof()) {
			cout << "Файл " << Name << " не содержит значений." << endl;
			Ferror << "Файл " << Name << " не содержит значений." << endl;
			Fi.close();
			continue;
		}

		cout << "\n\tФайл " << char('A' + i) << ".txt: " << endl;

		for (int ii = 0; !Fi.eof(); ii++) {
			Fi >> X;
			Array[i][ii] = X;
			cout << Array[i][ii] << endl;
		}
		if (Size[i] > Real_Size[i]) {
			cout << "Требуемая длинна больше файла! Будет приравнино к настоящей длине." << endl;
			Ferror << "Требуемая длинна больше файла! Будет приравнино к настоящей длине." << endl;
			Size[i] = Real_Size[i];
		}
		Fi.close();
	}

	int Choose = 0, Iter[2]{ 0,0 };

	cout << "\n\tВыходной файл:" << endl;

	for (; Iter[Choose] < Size[Choose]; Iter[Choose]++) {
		Choose = (Array[0][Iter[0]] < Array[1][Iter[1]]) ? 0 : 1;
		Fo << Array[Choose][Iter[Choose]] << endl;
		cout << Array[Choose][Iter[Choose]] << endl;
	}

	Choose = (Choose == 1) ? 0 : 1;

	for (; Iter[Choose] < Size[Choose]; Iter[Choose]++) {
		Fo << Array[Choose][Iter[Choose]] << endl;
		cout << Array[Choose][Iter[Choose]] << endl;
	}

	Ferror.close();
	Fo.close();

	return 0;
}
