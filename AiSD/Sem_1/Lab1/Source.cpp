#include "Header.h"

#include "Heap.h"
#include "Insertion.h"
#include "Merge.h"
#include "Selection.h"
#include "Bubble.h"
#include "Shell.h"
#include "Quick.h"



int main() {
	std::vector<int> Sampl{ 0, 12, -234, 1478, 332, -34534, 87 };
	QuickSort(Sampl);
	for (int& i : Sampl) std::cout << i << ' ';
	return 0;
}