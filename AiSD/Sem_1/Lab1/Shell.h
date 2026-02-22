#pragma once

#include "Header.h"

template<typename T>
void ShellSort(std::vector<T>& V) {
	for (size_t D = V.size() / 2; D > 0; D /= 2) {
		for (size_t i = D; i < V.size(); i++) {
			T Temp = V[i];
			size_t j = i;
			for (; j >= D && V[j - D] > Temp; j -= D)
				V[j] = V[j - D];
			V[j] = Temp;
		}
	}
}