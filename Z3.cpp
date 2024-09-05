/*я—»√ћјя –”“ќ…я—»√ћј
* Задание:
*			322,321 x^8 + 32,432 x^7 Ц x
*			9,09 x^6 + 6543,3 x^3 Ц 0,2132 x
* Автор: Редкокаша Роман Юрьевич				  Версия:1.1.1
* Группа: 4352
* Даты: начало: 04.09.2024 - конец:потом
*/
#include <stdio.h>
#include <locale>

using namespace std;

int main() {
	setlocale(0, "RUSSIAN");
	printf("%-*s", 20, "¬ведите X: ");

	double X;
	scanf_s("%lf", &X);

	printf("%-*s", 20, "\nПример №1\n");

	printf("%-*s", 20, "22,321X^8+32,432X^7-X = ((322,321X+32,432)X^6-1)X\n");

	double P = 322.321 * X + 32.432;
	printf("%-*s%.*lf", 20, "p1 = 322,321X+32,432 = ", 7, P);

	double Xn = X * X * X; // ^3
	Xn *= Xn; // X^6
	P = P * Xn - 1;
	printf("%-*s%.*lf", 20, "\np2 = p1 * X ^ 6 - 1 = ", 7, P);

	P *= X;
	printf("%-*s%.*lf", 20, "\np3 = p2 * X = ", 7, P);
	printf("%-*s%.*lf", 20, "\n322,321X^8 + 32,432X^7 - X = ", 7, P);

	printf("%-*s", 20, "\n\n////////////////////////\n\nПример №2\n");

	printf("%-*s", 20, "9,09X^6 + 6543,3X^3 - 0,2132X = ((9,09X^3 + 6543,3)X^2 - 0,2132)X\n");

	Xn = X * X;
	P = 9.09 * Xn * X + 6543.3;
	printf("%-*s%.*lf", 20, "p1 = 9,09X^3 + 6543,3 = ", 7, P);

	P = P * Xn - 0.2132;
	printf("%-*s%.*lf", 20, "\np2 = p1 * X^2 - 0,2132 = ", 7, P);

	P = P * X;
	printf("%-*s%.*lf", 20, "\np3 = p2 * X = ", 7, P);
	printf("%-*s%.*lf%c", 20, "\n9,09X^6 + 6543,3X^3 - 0,2132X = ", 7, P, '\n');

	system("pause");

	return 0;
}