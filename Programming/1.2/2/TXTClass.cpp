#include "TXTClass.h"

TXT::TXT(const char* Name) : F(std::fstream(Name, std::ios::in)) {
	//F << std::setiosflags(std::ios::skipws);
	if (!F.get(mark)) {
		is_good = false;
		return;
	}
	//if (!F.is_open()) {
	//	is_good = false;
	//	return;
	//}

	F >> scale_x;
	F >> scale_y;

	lines = new PosNSost[scale_y + 1];
	//std::cout << scale_x << ' ' << scale_y << ' ' << mark << std::endl;
	while (F.get(Temp) && Temp != '\n');
	lines[0].start = F.tellg();
	lines[0].is_end = false;
}

const size_t& TXT::GetScaleY() {
	return scale_y;
}

bool TXT::IsNotEnd() {
	return !is_end;
}

void TXT::GetScreen(Screen& Scr) {
	Scr.NewScr();
	for (size_t y = 0; y < scale_y; y++) {
		size_t Real_Size = 0;

		if (lines[y].is_end) {
			Scr[y].SetSize(Real_Size);
			continue;
		}
		F.clear();
		F.seekg(lines[y].start);

		while (Real_Size < scale_x && F.get(Temp) && Temp != '\n' && Temp != mark) {
			Real_Size++;
		}

		Scr[y].SetSize(Real_Size);

		F.clear();
		F.seekg(lines[y].start);

		for (size_t i = 0; i < Real_Size; i++) {
			F.get(Temp);
			Scr[y][i] = Temp;
		}

		if (!F.get(Temp) || Temp == '\n' || Temp == mark)
			lines[y].is_end = true;
		else
			F.seekg(-1, std::ios::cur);
		
		lines[y].start = F.tellg();

		while (Temp != '\n' && Temp != mark) {
			if (!F.get(Temp)) {
				is_good = false;
				break;
			}
		}

		if (!F.get(Temp)) {
			is_good = false;
			break;
		}
		F.seekg(-1, std::ios::cur);

		if (F.tellg() > lines[y + 1].start && is_good) {
			lines[y + 1].start = F.tellg();
			lines[y + 1].is_end = false;
		}
	}

	for (size_t i = 0; i < scale_y; i++) {
		if (!lines[i].is_end) return;
	}
	if (!is_good)
		is_end = true;

	lines[0].start = lines[scale_y].start;
	lines[0].is_end = false;

	Scr.is_last = true;
}
//void TXT::GetScreen(Screen& Scr) {
//	//std::cout << "Getting screen";
//	Scr.NewScr();
//	for (size_t y = 0; y < scale_y; y++) {
//		size_t Real_Size = 0;
//		//std::cout << '.';
//
//		if (lines[y].is_end) {
//			Scr[y].SetSize(Real_Size);
//			continue;
//		}
//		F.clear();
//		F.seekg(lines[y].start);
//
//		for (size_t i = 0; i < scale_x && F.get(Temp) && !(Temp == '\n' || Temp == mark); i++) {
//			//if (Temp == '\n' || Temp == mark) {
//			//	lines[y].is_end = true;
//			//	break;
//			//}
//			Real_Size++;
//		}
//
//		Scr[y].SetSize(Real_Size);
//		F.seekg(lines[y].start);
//		//F.getline(Scr[y].str, Real_Size);
//		for (size_t i = 0; i < Real_Size && F.get(Temp); i++) {
//			Scr[y][i] = Temp;
//			//std::cout << Temp;
//		}
//		//std::cout << '\n';
//		if (!F.get(Temp) || Temp == '\n' || Temp == mark)
//			lines[y].is_end = true;
//		else
//			F.seekg(-1, std::ios::cur);
//		lines[y].start = F.tellg();
//
//		while (Temp != '\n' && Temp != mark && F.get(Temp));
//
//		if (F.tellg() > lines[y + 1].start) {
//			lines[y + 1].start = F.tellg();
//			if (F.get())
//				lines[y + 1].is_end = false;
//		}
//		
//	}
//
//	for (size_t i = 0; i < scale_y; i++) {
//		if (!lines[i].is_end) return;
//	}
//	if (!is_good)
//		is_end = true;
//	for (size_t i = 0; i < scale_y; i++) {
//		lines[i].is_end = true;
//	}
//	lines[0].start = lines[scale_y].start;
//	Scr.is_last = true;
//}

void TXT::MakeScr(Screen& Scr) {
	Scr.NewScr(mark, scale_y);
}

TXT::~TXT() {
	delete[] lines;
}


void ScrData::AddPov(const char& C) {
	if (!str.str) return;
	char* i = &str[start];
	for (; *i != str.mark; i++);
	*i = C;
	*(i + 1) = str.mark;
	num -= shtraf - 1 - start;
	start = shtraf;
}

bool ScrData::AddNePov(const char& C) {
	if (!str.str) return false;
	size_t i = start;
	for (; str[i] != str.mark; i++) {
		if (str[i] == C) {
			shtraf = i + 1;
			return false;
		}
	}
	str[i] = C;
	str[i + 1] = str.mark;
	num++;
	return true;
}

//bool ScrData::AddNePov(const char& C) {
//	if (!str.str) return false;
//	char* i = str.str;
//	for (; *i != str.mark; i++) {
//		if (*i == C) return false;
//	}
//	*i = C;
//	*(i + 1) = str.mark;
//	num++;
//	return true;
//}

void ScrData::CreateChild(ScrData& StrD, size_t NowS) {
	if (!StrD.str.str) {
		str.SetSize(NowS);
		str[0] = str.mark;
		return;
	}
	str.mark = StrD.str.mark;
	num = StrD.num;
	str.SetSize(NowS + num);
	size_t i = 0;
	for (; StrD.str[i+ StrD.start] != str.mark; i++) {
		str[i] = StrD.str[i + StrD.start];
	}
	str[i] = str.mark;
}

void ScrData::Clear() {
	start = 0;
	str[0] = str.mark;
	num = 0;
}

ScrData& ScrData::operator= (ScrData& Str) {
	if (&Str == this) return *this;
	Str.start;
	str.mark = Str.str.mark;
	num = Str.num;
	str.SetSize(num);
	if (!Str.str.str) return *this;
	for (size_t i = 0; Str.str[i + Str.start] != str.mark; i++) {
		str[i] = Str.str[i + Str.start];
	}
	return *this;
}