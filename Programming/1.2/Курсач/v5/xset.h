#pragma once

#include "str.h"

struct XSet {
	STR str;
	XSet* next = nullptr;
};