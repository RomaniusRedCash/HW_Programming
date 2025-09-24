#include "fstream"
#include "iomanip"
//#include "iostream"

using namespace std;

fstream FProc("Procces.txt", ios::out), FProt("Protocol.txt", ios::out), F("Inp.txt", ios::in), FT("Temp.dat", ios::binary | ios::in | ios::out | ios::trunc);
size_t MaxC = 0, NumOfFig = 0;
const double Eps = 10E-2;

///////////////////////////////////////////////OutMain///////////////////////////////////////////////

const void OutMain(const size_t& Value, const char* EndStr) {
	FProc << Value << EndStr;
	FProt << "Вывод пользователю: " << Value << EndStr;
	return;
}
const void OutMain(const double& Value, const char* EndStr) {
	FProc << Value << EndStr;
	FProt << "Вывод пользователю: " << Value << EndStr;
	return;
}
const void OutMain(double*& Value, const char* Skoba, const char* EndStr) {
	FProc << Skoba[0] << Value[0] << ", " << Value[1] << Skoba[1] << EndStr;
	FProt << "Вывод пользователю: " << Skoba[0] << Value[0] << ", " << Value[1] << Skoba[1] << EndStr;
	return;
}
const void OutMain(const char* TXT) {
	FProc << TXT;
	FProt << "Вывод пользователю: " << TXT;
	return;
}

///////////////////////////////////////////////OutProt///////////////////////////////////////////////

const void OutProt(const size_t& Value, const char* EndStr) {
	FProt << Value << EndStr;
	return;
}
const void OutProt(const size_t& Value1, size_t& Value2, const char* Skoba, const char* EndStr) {
	FProt << Skoba[0] << Value1 << ", " << Value2 << Skoba[1] << EndStr;
	return;
}
const void OutProt(const double& Value, const char* EndStr) {
	FProt << Value << EndStr;
	return;
}
const void OutProt(double* Value, const char* Skoba, const char* EndStr) {
	FProt << Skoba[0] << Value[0] << ", " << Value[1] << Skoba[1] << EndStr;
	return;
}
const void OutProt(const char* TXT) {
	FProt << TXT;
	return;
}	
const void OutProt(const char& C, const char* EndStr) {
	FProt << C << EndStr;
	return;
}

///////////////////////////////////////////////OutProt///////////////////////////////////////////////

void OutEr(const char* TXT) {
	FProc << "\nError:\t" << TXT << "\n\n";
	FProt << "\nError:\t" << TXT << "\n\n";
	return;
}

///////////////////////////////////////////////Size and Read///////////////////////////////////////////////

bool ReadOnlyDouble(double& X, char& C) {
	if (!F.eof()) {
		F >> resetiosflags(ios::skipws);
		F >> C;
		OutProt("\tСчитан символ: ");
		OutProt(C, "\n");
		if (C >= '0' && C <= '9' || C == '-') {
			F.seekg(-1, ios::cur);
			F >> setiosflags(ios::skipws);
			F >> X;
			OutProt("\tСчитано число: ");
			OutProt(X, "\n");
			return true;
		}
	}
	return false;
}

size_t CalcSize() {
	if (!F.is_open()) {
		OutEr("Файла нет!");
		return 0;
		
	}
	if (!F.get()) {
		OutEr("Файл пуст!");
		return 0;
	}
	F.seekg(0, ios::beg);
	char C = '!';
	double S = 0;
	size_t RS = 0;
	OutProt("Чтение размера:\n");
	while (!F.eof())
		if (ReadOnlyDouble(S, C)) break;
	if (S <= 0) {
		OutEr("Размер меньше 0, не указан или равен 0!");
		return 0;
	}
	double X;
	size_t T;
	OutProt("Расчёт реального размера:\n");
	while (!F.eof() && RS < S) {
		T = 0;
		while (C != '\n' && !F.eof()) if (ReadOnlyDouble(X, C) && T < 2) {
			if (++T == 2){
				RS++;
				OutProt("\t\tРеальный размер переназначен на: ");
				OutProt(RS, "\n");
			}

		}
		C = '!';
	}
	if (S > RS) {
		OutEr("Размер приравнен к реальному, так как меньше указанного!\n");
		return RS;
	}
	OutProt("Размер либо больше, либо равен указаному.\n");
	return S;
}

void Read(double** A, const size_t& S) {
	F.clear();
	F.seekg(0, ios::beg);
	OutProt("Файл сброшен.\n");
	char C = '!';
	double X;
	OutProt("Пропуск размера:\n");
	while (!ReadOnlyDouble(X, C));
	size_t T, TT = 0;
	OutProt("Запись в массив:\n");
	while (!F.eof() && TT < S) {
		T = 0;
		while (C != '\n' && !F.eof())	if (ReadOnlyDouble(X, C) && T < 2) {
			A[TT][T++] = X;
			OutProt("\t\tВ позиции ");
			OutProt(TT, T, "[]", " записано число: ");
			OutProt(X, "\n");
		}
		C = '!';
		TT+= (T == 2) ? 1 : 0;
	}
	return;
}

///////////////////////////////////////////////Uslovia///////////////////////////////////////////////


//double& Min(double& A, double& B) {
//	return A < B ? A : B;
//}
//
//double& Max(double& A, double& B) {
//	return A > B ? A : B;
//}
size_t& Max(size_t& A, size_t& B) {
	return A > B ? A : B;
}

double* GetVector(double*& A1, double*& A2) {
	double* A = new double[2]{ (A2[0] - A1[0]), (A2[1] - A1[1]) };
	//double A[2]{ (A2[0] - A1[0]), (A2[1] - A1[1]) };
	OutProt("\t\t\tОтрезок с координатами точки 1 ");
	OutProt(A1, "()", " и ");
	OutProt("точки 2 ");
	OutProt(A2, "()", " ");
	OutProt("преобразован в вектор ");
	OutProt(A, "{}", "\n");
	return A;
}

double Len(double*& A1, double*& A2) {
	double* A = GetVector(A1, A2);
	double Out = sqrt(pow(A[0], 2) + pow(A[1], 2));
	OutProt("\t\t\tДлина вектора ");
	OutProt(A, "{}", " = ");
	OutProt(Out, "\n");
	delete A;///////////////////////
	return Out;
}

bool EqualLen(double*& A1, double*& A2, double*& B1, double*& B2) {
	return abs(Len(A1, A2) - Len(B1, B2)) <= Eps;
}

bool IsNormal(double*& A1, double*& A2, double*& B1, double*& B2) {
	double* A = GetVector(A1, A2);
	double* B = GetVector(B1, B2);
	bool Out = abs(A[0] * B[0] + A[1] * B[1]) <= Eps;
	OutProt("\t\t\tВектор ");
	OutProt(A, "{}", (Out ? " перпендикулярен" : " не перпендикулярен вектору "));
	OutProt(B, "{}", "\n");
	delete A;///////////////////////
	delete B;;///////////////////////
	return Out;
}

bool CanUse(size_t*& B, const size_t& X, const short& Pos) {
	OutProt("\t\t\tПроверка наличия точки ");
	OutProt(X, " в фигуре:\n");
	for (int i = 0; i < Pos; i++) {
		OutProt("\t\t\t\tСравнение с ");
		OutProt(B[i], ":\n");
		if (B[i] == X) {
			OutProt("\t\t\tЕсть в фигуре.\n");
			return false;
		}
	}
	OutProt("\t\t\tНет в фигуре.\n");
	return true;
}

bool Spec3(double*& A1, double*& A2, double*& A3) {
	return pow(Len(A1, A3), 2) + pow(Len(A2, A3), 2) <= pow(Len(A1, A2), 2) 
		&& Len(A1, A3) + Len(A2, A3) > Len(A1, A2);
}

double GetYfromX(double*& A1, double*& A2, double& X) {
	OutProt("\t\t\t\tЗначение Y для прямой ");
	OutProt(A1, "()", " ");
	OutProt(A2, "()", " на координате оси X = ");
	OutProt(X, ":  ");
	double K = 0;
	if (A1[0] - A2[0]) K = (A1[1] - A2[1]) / (A1[0] - A2[0]);
	double Z = K * X + A1[1] - K * A1[0];
	OutProt(Z, "\n");
	return Z;
}
bool BetweenY(double*& A1, double*& A2, double*& A3, double*& A4, double*& B) {
	double Y1 = GetYfromX(A1, A2, B[0]), Y2 = GetYfromX(A3, A4, B[0]);
	OutProt("\t\t\t\tТочка ");
	OutProt(B, "()", " между Y фигуры ");
	OutProt(A1, "()", " ");
	OutProt(A2, "()", " "); 
	if (A2 != A3)	OutProt(A3, "()", " "); 
	OutProt(A4, "()", " "); 
	if (B[1] <= Y1 && B[1] >= Y2 || B[1] >= Y1 && B[1] <= Y2) {
		OutProt("находится.\n");
		return true;
	}
	OutProt("не находится.\n");
	return false;
}
bool BetweenY(double*& A1, double*& A2, double*& A3, double*& B) {
	return BetweenY(A1, A2, A2, A3, B);
}
bool BetweenX(double*& A1, double*& A2, double X) {
	OutProt("\t\t\t\tТочка с X координатой ");
	OutProt(X, " между X координатами прямой ");
	OutProt(A1, "()", " ");
	OutProt(A2, "()", " ");
	if (X >= A1[0] && X <= A2[0] || X <= A1[0] && X >= A2[0]) {
		OutProt("находится.\n");
		return true;
	}
	OutProt("не находится.\n");
	return false;
}

bool Prinadlejit(double*& A1, double*& A2, double*& A3, double*& B) {
	bool b = false;
	OutProt("\t\t\tПроверка принадлежности точки ");
	OutProt(B, "()", " фигуре: ");
	OutProt(A1, "()", " ");
	OutProt(A2, "()", " ");
	OutProt(A3, "()", ":\n"); 
	if (BetweenX(A1, A2, B[0]) && BetweenX(A2, A3, B[0])) b = BetweenY(A1, A2, A3, B);
	else
		if (BetweenX(A2, A3, B[0]) && BetweenX(A3, A1, B[0])) b = BetweenY(A2, A3, A1, B);
		else
			if (BetweenX(A3, A1, B[0]) && BetweenX(A1, A2, B[0])) b = BetweenY(A3, A1, A2, B);
	OutProt("\t\t\t\tТочка с  ");
	OutProt(B, "()", (b ? " принадлежит фигуре.\n" : " не принадлежит фигуре.\n"));
	return b;
}
bool Prinadlejit(double*& A1, double*& A2, double*& A3, double*& A4, double*& B) {
	bool b = false;
	OutProt("\t\t\tПроверка принадлежности точки ");
	OutProt(B, "()", " фигуре: ");
	OutProt(A1, "()", " ");
	OutProt(A2, "()", " ");
	OutProt(A3, "()", " ");
	OutProt(A4, "()", ":\n");
	if (BetweenX(A1, A2, B[0]) && BetweenX(A2, A3, B[0])) b = BetweenY(A1, A2, A3, B);
	else
		if (BetweenX(A2, A3, B[0]) && BetweenX(A3, A4, B[0])) b = BetweenY(A2, A3, A4, B);
		else
			if (BetweenX(A3, A4, B[0]) && BetweenX(A4, A1, B[0])) b = BetweenY(A3, A4, A1, B);
			else
				if (BetweenX(A4, A1, B[0]) && BetweenX(A1, A2, B[0])) b = BetweenY(A4, A1, A2, B);			
				else
					if (BetweenX(A1, A2, B[0]) && BetweenX(A3, A4, B[0])) b = BetweenY(A1, A2, A3, A4, B);
	OutProt("\t\t\t\tТочка ");
	OutProt(B, "()", (b ? " принадлежит фигуре." : " не принадлежит фигуре.\n"));
	return b;
}

bool Peresec(double*& A1, double*& A2, double*& A3, double*& B) {
	OutProt("\t\t\tПрямая ");
	OutProt(A1, "()", " ");
	OutProt(A2, "()", " ");
	OutProt("прямую ");
	OutProt(A3, "()", " ");
	OutProt(B, "()", " ");
	double K1 = (A1[1] - A2[1]) / (A1[0] - A2[0]), K2 = (A3[1] - B[1]) / (A3[0] - B[0]);
	double X = (K1 * A1[0] - A1[1] - (K2 * A3[0] - A3[1])) / (K1 - K2);
	if (BetweenX(A1, A2, X) && BetweenX(A3, B, X)) {
		OutProt("пересекает.\n");
		return true;
	}
	OutProt("не пересекает.\n");
	return false;
}

bool IsCorrect(double**& A, size_t*& B, const short& Pos, size_t& i) {
	if (Pos > 9) return false;
	switch (Pos + 1)
	{
	case 10:
		if (EqualLen(A[B[1]], A[B[2]], A[B[8]], A[i])
			&& IsNormal(A[B[2]], A[B[3]], A[B[8]], A[i]))	return true;
		break;
	case 9:
		if (EqualLen(A[B[2]], A[B[3]], A[B[7]], A[i])
			&& IsNormal(A[B[1]], A[B[2]], A[B[7]], A[i])) return true;
		break;
	case 8:
		if (EqualLen(A[B[1]], A[B[2]], A[B[6]], A[i])
			&& IsNormal(A[B[2]], A[B[3]], A[B[6]], A[i])) return true;
		break;
	case 7:
		if (EqualLen(A[B[0]], A[B[1]], A[B[5]], A[i])
			&& IsNormal(A[B[2]], A[B[3]], A[B[4]], A[i])) return true;
		break;
	case 6:
		if (EqualLen(A[B[0]], A[B[1]], A[B[4]], A[i])
			&& IsNormal(A[B[1]], A[B[2]], A[B[0]], A[i])
			&& Len(A[B[0]], A[i]) > Len(A[B[2]], A[B[3]])) return true;
		break;
	case 5:
		if (EqualLen(A[B[1]], A[B[2]], A[B[3]], A[i])
			&& IsNormal(A[B[2]], A[B[3]], A[B[3]], A[i])) return true;
		break;
	case 4:
		if (IsNormal(A[B[1]], A[B[2]], A[B[2]], A[i])
			&& Len(A[B[2]], A[i]) > 0
			&& !Prinadlejit(A[B[0]], A[B[1]], A[B[2]], A[i])
			&& !Peresec(A[B[0]], A[B[1]], A[B[2]], A[i])) return true;
		break;
	case 3:
		if (Spec3(A[B[0]], A[B[1]], A[i])
			&& Len(A[B[1]], A[i]) > 0) return true;
		break;
	default:
		return true;
		break;
	}
	return false;
}

///////////////////////////////////////////////Save///////////////////////////////////////////////

bool NotInSaveF(size_t*& B) {
	OutProt("\tПроверка на повтор фигуры:\n");
	FT.seekg(0, ios::beg);
	FT.clear();
	size_t B2[10];
	bool b = true;
	for (size_t i = 0; FT.read(reinterpret_cast<char*>(B2), sizeof(B) * 10); i++) {
		OutProt("\t\t№:");
		OutProt(i, ":\n");
		FT.seekg(sizeof(B), ios::cur);
		b = false;
		for (size_t j : B2)
			if (CanUse(B, j, 10)) b = true;
		if (!b) {
			OutProt("\t\tФигура уже найдена.\n");
			return b;
		}
	}
		OutProt("\t\tФигура новая.\n");
		return b;
}
void SaveF(size_t*& B) {
	FT.seekp(0, ios::end);
	FT.clear();
	OutProt("Сохранение фигуры №");
	OutProt(NumOfFig, ".\n");
	FT.write(reinterpret_cast<char*>(B), sizeof(B) * 11);
	NumOfFig++;
}

///////////////////////////////////////////////Out Figure///////////////////////////////////////////////

void OutFig(double**& A, size_t*& B) {
	OutProt("Найдена фигура:\n");
	for (short i = 0; i < 10; i++) {
		OutProt("\t Точка №");
		OutProt(B[i], ": ");
		OutProt(A[B[i]], "()", "\n");
	}
	OutProt("\tСодержит: ");
	OutProt(B[10], " точек.\n");
}
void OutFigMain(double**& A, size_t*& B) {
	OutMain("Найдена фигура:\n");
	for (short i = 0; i < 10; i++) {
		OutMain("\t Точка №");
		OutMain(B[i], ": ");
		OutMain(A[B[i]], "()", "\n");
	}
	OutMain("\tСодержит: ");
	OutMain(B[10], " точек.\n");
}

void FindMax(double**& A, size_t* B, const size_t& S) {
	FT.seekg(0, ios::beg);
	FT.clear();
	OutMain("Максимум: ");
	OutMain(MaxC, ". Поиск фигуры:\n");
	for (size_t i = 0; FT.read(reinterpret_cast<char*>(B), sizeof(B) * 11); i++) {
		if (B[10] == MaxC) {
			OutMain("\tФигура и точки, которые она содержит №");
			OutMain(i, ":\n");
			OutFigMain(A, B);
			for (size_t i = 0; i < S; i++)
				if (CanUse(B, i, 10)
					&& (Prinadlejit(A[B[0]], A[B[1]], A[B[9]], A[i])
						|| Prinadlejit(A[B[2]], A[B[3]], A[B[7]], A[B[8]], A[i])
						|| Prinadlejit(A[B[4]], A[B[5]], A[B[6]], A[i]))) {
					OutMain("\t\tТочка №");
					OutMain(i, ": ");
					OutMain(A[i], "()", "\n");
				}
		}
	}
}

///////////////////////////////////////////////Procces///////////////////////////////////////////////

size_t& Schot(double**& A, size_t* B, const size_t& S) {
	B[10] = 0;
	OutProt("\tСчёт точек в фигуре:\n");
	for (size_t i = 0; i < S; i++) {
		OutProt("\t\tТочка ");
		OutProt(i, ": ");
		OutProt(A[i], "()", ":\n");
		if (CanUse(B, i, 10)
			&& (Prinadlejit(A[B[0]], A[B[1]], A[B[9]], A[i])
				|| Prinadlejit(A[B[2]], A[B[3]], A[B[7]], A[B[8]], A[i])
				|| Prinadlejit(A[B[4]], A[B[5]], A[B[6]], A[i]))) B[10]++;
	}
	OutFig(A, B);
	return B[10];
}

void SubProcces(double**& A, size_t* B, const size_t& S, short Pos) {
	OutProt("\tПоиск точки №");
	OutProt(static_cast<size_t>(Pos), ":\n");
	for (size_t i = 0; i < S; i++) {
		OutProt("\t\tПроверка точки ");
		OutProt(i, ":\n");
		if (CanUse(B, i, Pos) && Pos < 10 && IsCorrect(A, B, Pos, i)) {
			OutProt("\tТочка ");
			OutProt(i, " установлена как №");
			OutProt(static_cast<size_t>(Pos), ".\n");
			B[Pos] = i;
			if (Pos >= 9) {
				if (NotInSaveF(B)) {
					MaxC = Max(Schot(A, B, S), MaxC);
					SaveF(B);
				}
			} else
				SubProcces(A, B, S, Pos + 1);
		}
	}
	OutProt("\tВыход из цикла для точки №");
	OutProt(static_cast<size_t>(Pos), ".\n");
	if (Pos) {
		OutProt("\tВвозврат к точке №");
		OutProt(static_cast<size_t>(Pos - 1), ":\n");
	}
	return;
}

void Procces(double**& A, size_t& S) {
	size_t B[11];
	OutProt("Поиск фигры:\n");
	SubProcces(A, B, S, 0);
	FindMax(A, B, S);
	return;
}

int main() {
	setlocale(LC_ALL, "RUSSIAN");
	FProc.setf(ios::fixed);
	FProt.setf(ios::fixed);
	FProt.precision(7);
	FProt.precision(7);
	size_t S = CalcSize();
	double** A = new double* [S];
	for (int i = 0; i < S; i++) A[i] = new double[2];
	Read(A, S);
	Procces(A, S);
	FProt.close();
	FProc.close();
	FT.close();
	F.close();
	for (int i = 0; i < S; i++) delete[] A[i];
	return 0;
}