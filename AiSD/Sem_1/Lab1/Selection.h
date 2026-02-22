#pragma once

#include "Header.h"

template<typename T>
void SelectionSort(std::vector<T>& V) {
	for (ITER i = V.begin(); i != V.end() - 1; i++) {
		ITER pMin = i;
		for (ITER j = i + 1; j != V.end(); j++)
			if (*pMin > *j)	pMin = j;
		std::swap(*pMin, *i);
	}
}