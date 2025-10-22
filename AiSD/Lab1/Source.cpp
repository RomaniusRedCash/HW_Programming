#include "Header.h"

#include "Heap.h"
#include "Insertion.h"
#include "Merge.h"
#include "Selection.h"



int main() {
	std::vector<int> Sempl{ 0, 12, -234, 1478, 332, -34534, 87 };
	HeapSort(Sempl);
	for (int& i : Sempl) std::cout << i << ' ';
	return 0;
}