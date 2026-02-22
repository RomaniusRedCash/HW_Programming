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
	int Size[2]{ 0, 0 }, Real_Size[2]{ 0, 0 }; //int*
	double Array[2][100], ArrayOut[200]; //double*
	
	setlocale(LC_ALL, "RUSSIAN");
	cout.setf(ios::fixed);
	cout.precision(7);
	cout <<
		"Задание:\n" <<
		"\tДаны две последовательности значений a1 <= a2 <= ... <= an и b1 <= b2 <= ... <= bm.\n" <<
		"\tСформировать из них последовательность значений с1 <= с2 <= ... <= сn + m.\n\n" <<
		"Автор: X8X8L				  Версия: 4.1\n" <<
		"Группа: 1488\n" <<
		"Даты: начало: 23.10.2024 - конец: 23.10.2024\n" << endl;
	Fo <<
		"Задание:\n" <<
		"\tДаны две последовательности значений a1 <= a2 <= ... <= an и b1 <= b2 <= ... <= bm.\n" <<
		"\tСформировать из них последовательность значений с1 <= с2 <= ... <= сn + m.\n\n" <<
		"Автор: X8X8L				  Версия: 4.1\n" <<
		"Группа: 1488\n" <<
		"Даты: начало: 23.10.2024 - конец: 23.10.2024\n" << endl;


	for (int i = 0; i < 2; i++) {
		Fi.close();
		char Name[] = "A.txt"; //char*
		Name[0] += i;
		Fi.open(Name, ios::in);
		if (!Fi.is_open()) {
			cout << "Файл " << Name << " не помещён в корневой каталог программы." << endl;
			Ferror << "Файл " << Name << " не помещён в корневой каталог программы." << endl;
			Fo << "Файл " << Name << " не помещён в корневой каталог программы." << endl;
			continue;
		}
		cout << "\n\tФайл " << Name << ".txt: " << endl;
		Fo << "\n\tФайл " << Name << ".txt: " << endl;

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

		while (Fi >> X) {
			Real_Size[i]++;
			if (Real_Size[i] > 100) { //?1
				cout << "Длина файла больше 100, что дальше этого значения - учитываться не будет!" << endl;
				Ferror << "Длина файла больше 100, что дальше этого значения - учитываться не будет!" << endl;
				Fo << "Длина файла больше 100, что дальше этого значения - учитываться не будет!" << endl;
				break;
			}
		}
		Real_Size[i]--;

		Fi.clear();
		Fi.seekg(0, ios::beg);
		Fi >> Size[i];
		if (Size[i] < 0)
		{
			cout << "Значение чисел отрицательно, было приравнено к нулю." << endl;
			Ferror << "Значение чисел отрицательно, было приравнено к нулю." << endl;
			Fo << "Значение чисел отрицательно, было приравнено к нулю." << endl;
			Size[i] = 0;
			continue;
		}

		for (int ii = 0; !Fi.eof() && ii < Real_Size[i]; ii++) { //?2
			Fi >> X;
			Array[i][ii] = X;
			cout << Array[i][ii] << endl;
			Fo << Array[i][ii] << endl;
		}
		if (Size[i] > Real_Size[i]) { //?3
			cout << "Требуемая длинна больше файла! Будет приравнено к настоящей длине." << endl;
			Ferror << "Требуемая длинна больше файла! Будет приравнено к настоящей длине." << endl;
			Fo << "Требуемая длинна больше файла! Будет приравнено к настоящей длине." << endl;
			Size[i] = Real_Size[i];
		}
	}
	Fi.close();

	int Choose = 0, Iter[2]{ 0,0 }, SubIter = 0; //int*2

	cout << "\n\tВыходной файл:" << endl;
	Fo << "\n\tВыходной файл:" << endl;

	if (Real_Size[0] != 0 && Real_Size[1] != 0) //?5
		for (; Iter[Choose] < Size[Choose]; Iter[Choose]++, SubIter++) { //?4
			Choose = (Array[0][Iter[0]] < Array[1][Iter[1]]) ? 0 : 1; //A3
			ArrayOut[SubIter] = Array[Choose][Iter[Choose]]; //A4
			Fo << ArrayOut[SubIter] << endl;
			cout << ArrayOut[SubIter] << endl;
		}
	else
		Choose = (Real_Size[0] < Real_Size[1]) ? 0 : 1; // A5

	Choose = (Choose == 1) ? 0 : 1; //A6

	for (; Iter[Choose] < Size[Choose]; Iter[Choose]++, SubIter++) { //?4
		ArrayOut[SubIter] = Array[Choose][Iter[Choose]]; // A4
		Fo << ArrayOut[SubIter] << endl;
		cout << ArrayOut[SubIter] << endl;
	}

	Ferror.close();
	Fo.close();

	return 0;
}
