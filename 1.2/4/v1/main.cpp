#define _CRTDBG_MAP_ALLOC
#include "func.h"
#include <crtdbg.h>

int main() {
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);

	_CrtDumpMemoryLeaks();
	return 0;
}