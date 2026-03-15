#pragma once

#include <fstream>
#include "TXTClass.h"
#include "STRsClass.h"

void PrintY(Screen& Scr, std::fstream& FO);
void PrintX(STR& Str, std::fstream& FO);

void Procces(TXT& Txt, Screen& Scr, std::fstream& FO);

ScrData& Min(ScrData& S1, ScrData& S2);
ScrData& Max(ScrData& S1, ScrData& S2);
void MiniMax(ScrData& S1, ScrData* S2, const size_t& Size);

void PrintAns(ScrData& ScrD, std::fstream& FO);