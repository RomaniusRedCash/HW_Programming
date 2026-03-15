/*я—»√ћјя –”“ќ…я—»√ћј
* Задание:
* u(i)=(-1)^ix^(4i)/(4i)!, Σu
* Автор: X8X8L				  Версия: 3.1
* Группа: 1488
* Даты: начало: 10.10.2024 - конец: 10.10.2024
*/
#include "iostream"
#include "fstream"
#include "iomanip"
#include "cmath"

using namespace std;

int main() {
	setlocale(LC_ALL, "RUSSIAN");
	cout.setf(ios::scientific);
	//cout.setf(ios::fixed);
	cout.precision(7);
	cout << "Задание:\n"
		<< "u(i) = (-1) ^ ix ^ (4i) / (4i)!, Eu\n"
		<< "Автор: X8X8L\tВерсия : 3.1\n"
		<< "Группа: 1488\n"
		<< "Даты : начало: 10.10.2024 - конец : 10.10.2024" << endl;

	fstream F("log.txt", ios::out);
	unsigned long long i = 0;
	double Eps = 0;
	i = 0;

	cout << "Введите 0 < Eps < 1: ";
	while (Eps <= 0 || Eps >= 1) {
		if (++i > 3) {
			Eps = 1E-10;
			cout << "Попытки кончались. Eps будет равен: " << Eps << endl;
			break;
		}
		if (i-1) { cout << "Неверное значение! Попыток осталось: " << 4 - i << "\nВведите Eps: "; }
		cin >> Eps;
	}
	cout << "Введите x: ";
	double x;
	cin >> x;
	F << "X = " << x << " Eps = " << Eps << '\n';
	x = -pow(x, 4);
	double U = 1, Eu = 0;
	cout << "---------------------------------------" << endl;
	cout << noshowpos << '|' << setw(5) << left << 'N' << '|' << setw(15) << 'U' << '|' << setw(15) << "Σu" << '|' << endl;
	cout << "---------------------------------------" << endl;
	F << "---------------------------------------" << endl;
	F << noshowpos << '|' << setw(5) << left << left << 'N' << '|' << setw(15) << 'U' << '|' << setw(15) << "Σu" << '|' << endl;
	F << "---------------------------------------" << endl;
	for (i = 1; abs(U) >= Eps; i++)
	{
		U *= x / (4 * i * (4 * i - 1) * (4 * i - 2) * (4 * i - 3));
		Eu += U;
		cout << noshowpos << '|' << setw(5) << left << i << '|' << setw(15) << showpos << U << '|' << setw(15) << Eu << '|' << endl;
		cout << noshowpos << "---------------------------------------" << endl;

		F << noshowpos << '|' << setw(5) << left << i << '|' << setw(15) << showpos << U << '|' << setw(15) << Eu << '|' << endl;
		F << noshowpos << "---------------------------------------" << endl;

		if (i % 100 == 0) {
			system("pause");
		}
	}

	F.close();

	return 0;
}
