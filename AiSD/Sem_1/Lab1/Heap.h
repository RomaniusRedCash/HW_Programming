#pragma once

#include "Header.h"

template<typename T>
void Heapfy(std::vector<T>& V, const size_t& Size, const size_t& Pivot) {
	size_t Temp = Pivot * 2 + 1, Largest = Pivot;

	if (Temp >= Size) return;
	if (V[Largest] < V[Temp]) Largest = Temp;

	if (++Temp >= Size) return;
	if (V[Largest] < V[Temp]) Largest = Temp;

	if (Pivot != Largest) {
		std::swap(V[Pivot], V[Largest]);
		return Heapfy(V, Size, Largest);
	}
}

//template<typename T>
//void MakeHeap(std::vector<T>& V) {
//	for (size_t i = V.size() / 2; i > 0; i--) Heapfy(V, V.size(), i);
//}

template<typename T>
void HeapSort(std::vector<T>& V) {
	if (V.size() < 2) return;

	for (size_t i = V.size() / 2; i > 0; i--) Heapfy(V, V.size(), i - 1); // MakeHeap(V);

	for (size_t i = V.size() - 1; i > 0; i--) {
		std::swap(V[i], V.front());
		Heapfy(V, i, 0);
	}
}