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
	fstream Fi, Fo("out.txt", ios::out), Ferror("error.txt", ios::out); //fstream*
	int Real_Size[2]{ 0, 0 }, SumSize = 0; //int*
	double** Array = new double* [2] {nullptr, nullptr}; //double*
	double* ArrayOut = nullptr ; //double**
	
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
	Fo <<
		"Задание:\n" <<
		"\tДаны две последовательности значений a1 <= a2 <= ... <= an и b1 <= b2 <= ... <= bm.\n" <<
		"\tСформировать из них последовательность значений с1 <= с2 <= ... <= сn + m.\n\n" <<
		"Автор: X8X8L				  Версия: 4.2\n" <<
		"Группа: 1488\n" <<
		"Даты: начало: 23.10.2024 - конец: 23.10.2024\n" << endl;

	

	for (int i = 0; i < 2; i++) {
		Fi.close();
		char Name[] = "A.txt";
		Name[0] += i;
		Fi.open(Name, ios::in);
		if (!Fi.is_open()) {
			cout << "Файл " << Name << " не помещён в корневой каталог программы." << endl;
			Ferror << "Файл " << Name << " не помещён в корневой каталог программы." << endl;
			Fo << "Файл " << Name << " не помещён в корневой каталог программы." << endl;
			continue;
		}

		cout << "\n\tФайл " << char('A' + i) << ".txt: " << endl;
		Fo << "\n\tФайл " << char('A' + i) << ".txt: " << endl;

		char C;
		if (!Fi.get(C)) {
			cout << "Файл " << Name << " не содержит значений." << endl;
			Ferror << "Файл " << Name << " не содержит значений." << endl;
			Fo << "Файл " << Name << " не содержит значений." << endl;
			continue;
		}
		Fi.clear();
		Fi.seekg(0, ios::beg);

		double X;

		while (!Fi.eof()) {
			Fi >> X;
			Real_Size[i]++;
		}

		Fi.clear();
		Fi.seekg(0, ios::beg);

		*(Array + i) = new double[Real_Size[i]]; // A1
		SumSize += Real_Size[i];

		for (int ii = 0; !Fi.eof(); ii++) { //?2
			Fi >> X;
			*(*(Array + i) + ii) = X;
			cout << *(*(Array + i) + ii) << endl;
			Fo << *(*(Array + i) + ii) << endl;
		}
	}
	Fi.close();

	int Choose = 0, Iter[2]{ 0,0 }, SubIter = 0; //int*2
	ArrayOut = new double[SumSize]; //A2

	cout << "\n\tВыходной файл:" << endl;
	Fo << "\n\tВыходной файл:" << endl;

	if (Real_Size[0] != 0 && Real_Size[1] != 0)
		for (; Iter[Choose] < Real_Size[Choose]; Iter[Choose]++, SubIter++) { //?4
			Choose = (*(*(Array + 0) + Iter[0]) < *(*(Array + 1) + Iter[1])) ? 0 : 1; //A3
			*(ArrayOut + SubIter) = *(*(Array + Choose) + Iter[Choose]); //A4
			Fo << *(ArrayOut + SubIter) << endl;
			cout << *(ArrayOut + SubIter) << endl;
		}
	else
		Choose = (Real_Size[0] < Real_Size[1]) ? 0 : 1;

	Choose = (Choose == 1) ? 0 : 1; //A5

	for (; Iter[Choose] < Real_Size[Choose]; Iter[Choose]++, SubIter++) {
		*(ArrayOut + SubIter) = *(*(Array + Choose) + Iter[Choose]); //A4
		Fo << *(ArrayOut + SubIter) << endl;
		cout << *(ArrayOut + SubIter) << endl;
	}

	for (int i = 0; i < 2; i++)
		if (*(Array + i))
			delete[] *(Array + i);
	delete[] Array;
	delete[] ArrayOut;

	Ferror.close();
	Fo.close();

	return 0;
}
