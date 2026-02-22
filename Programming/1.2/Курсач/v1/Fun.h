#pragma once

//#include "ContentForm.h"
#include "Global.h"
#include "Menu.h"




bool Read();
Object<PosM>* GetMarsh();
template<typename T>
T* GetObjFromF(CustomSet<T>& CS);
void SkipSpace();
bool IsClose();
bool IsErr(const bool& b);

void ShowContent();