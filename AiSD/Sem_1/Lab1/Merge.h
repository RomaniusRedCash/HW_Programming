#pragma once

#include "Header.h"

template<typename T>
void Merge(std::vector<T>& V, ITER left, ITER right, ITER Mid) {

	std::vector<T> Vleft(left, Mid);
	std::vector<T> Vright(Mid, right);
	size_t j = 0, l = 0;

	ITER i = left;

	while (j < Vleft.size() && l < Vright.size() && i != right) {
		if (Vleft[j] <= Vright[l]) *i = Vleft[j++];
		else *i = Vright[l++];
		i++;
	}

	while (j < Vleft.size() && i != right)
		*i++ = Vleft[j++];

	while (l < Vright.size() && i != right)
		*i++ = Vright[l++];


}

template<typename T>
void MergeSort(std::vector<T>& V, ITER left, ITER right) {


	if (right - left <= 1) return;

	ITER Mid = left + (right - left) / 2;

	MergeSort(V, left, Mid);
	MergeSort(V, Mid, right);

	return Merge(V, left, right, Mid);
}

template<typename T>
void MergeSort(std::vector<T>& V) {
	return MergeSort(V, V.begin(), V.end());
}