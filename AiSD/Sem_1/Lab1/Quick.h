#pragma once

#include "Header.h"

template<typename T>
void Quick(std::vector<T>& V, size_t start, size_t end) {
	if (end - start < 1) return;
	T master = V[(start + end) / 2];
	size_t l = start, r = end;
	while (true) {
		while (V[l] < master) l++;
		while (V[r] > master) r--;
		if (l <= r) {
			std::swap(V[l], V[r]);
			if (r > 0) r--;
			l++;
		}
		else break;
	}
	if (r > start) Quick(V, start, r);
	if (l < end) Quick(V, l, end);
}

template<typename T>
void QuickSort(std::vector<T>& V) {
	Quick(V, 0, V.size() - 1);
}