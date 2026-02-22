#pragma once

#include "Header.h"

template<typename T>
void BubbleSort(std::vector<T>& V) {
	if (V.size() < 2) return;
	bool isSwapped = true;
	for (size_t i = 1; i < V.size() && isSwapped; i++) {
		isSwapped = false;
		for (ITER j = V.begin(); j != V.end() - i; j++)
			if (*j > *(j + 1)) {
				std::swap(*j, *(j + 1));
				isSwapped = true;
			}
	}
}