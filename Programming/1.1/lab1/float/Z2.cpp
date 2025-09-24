/*�������������������
* �������:
*			322,321 x^8 + 32,432 x^7 - x
*			9,09 x^6 + 6543,3 x^3 - 0,2132 x
* �����: ��������� ����� �������				  ������:1.2.1
* ������: 4352
* ����: ������: 04.09.2024 - �����:�����
*/
#include <iostream>
#include <iomanip>

using namespace std;

int main() {

	cout.setf(ios::fixed);
	cout.precision(7);
	setlocale(0, "RUSSIAN");

	cout << setiosflags(ios::left) << setw(20)
		<< "�������:\n322,321x^8 + 32,432x^7 - x\n9,09x^6 + 6543,3 x^3 - 0,2132x\n" <<
		"�����: ��������� ����� �������\t������ : 1.2.1\n������ : 4352\n���� : ������ : 04.09.2024 - ����� : �����\n";

	float X;
	cout << setiosflags(ios::left) << setw(20)
		<< "������� X: ";
	cin >> X;

	cout << "\n������ �1" << endl;

	float P = 322.321 * X + 32.432;

	float Xn = X * X * X; // ^3
	Xn *= Xn; // X^6
	P = P * Xn - 1;

	P *= X;

	cout << setiosflags(ios::left) << setw(20)
		<< "322,321X ^ 8 + 32,432X ^ 7 - X = " << P << endl;

	float P2 = P;

	/////////////////////////////////////////////////////////////
	cout << "\n////////////////////////\n\n" << "������ �2" << endl;
	/////////////////////////////////////////////////////////////

	Xn = X * X;
	P = 9.09 * Xn * X + 6543.3;

	P = P * Xn - 0.2132;

	P = P * X;

	cout << setiosflags(ios::left) << setw(20)
		<< "9,09X^6 + 6543,3X^3 - 0,2132X = " << P << endl;
	
	cout << setiosflags(ios::left) << setw(20)
		<< "�������" << P2/P << endl;

	system("pause");

	return 0;
}