#pragma once

#include "Header.h"

template<typename T>
void InsertionSort(std::vector<T>& V) {
	for (ITER i = V.begin() + 1; i != V.end(); i++) {
		for (ITER j = i; j > V.begin() && *j < *(j - 1); j--)
			std::swap(*j, *(j - 1));
	}
}