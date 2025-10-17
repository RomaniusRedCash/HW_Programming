#include <iostream>
#include <vector>




template<typename T>
void Merge(std::vector<T>& V, typename std::vector<T>::iterator& left, typename std::vector<T>::iterator& right){
	std::vector<T> Vleft = {V.begin() + left, V.begin() + (right + left) / 2};
	std::vector<T> Vright = {V.begin() + (right + left) / 2, V.begin() + right};
	size_t j = 0, l =0;
	for (typename std::vector<T> i = V.begin() + left; i < V.begin() + right; i++) {
		if (Vleft[j] <= Vright[l]) *i = Vleft[j++];
		else *i = Vright[l++];
	}




}

template<typename T>
void MergeSort(std::vector<T>& V, typename std::vector<T>::iterator& left, typename std::vector<T>::iterator& right){
	if (right - left <= 1) return;
	MergSort(V, left, (right + left) / 2);
	MergSort(V, (right + left) / 2 + 1, right);

	return Merge(V, left,right);
}


















template<typename T>
void Swap(T& X, T& Y) {
	T Temp = X;
	X = Y;
	Y = Temp;
}

template<typename T>
void Choosing(std::vector<T>& V) {
	for (typename std::vector<T>::iterator i = V.begin(); i != V.end(); i++) {
		typename std::vector<T>::iterator pMin = i;
		for (typename std::vector<T>::iterator j = i + 1; j != V.end(); j++)
			if (*pMin > *j)	pMin = j;
		Swap(*pMin, *i);
	}
}

template<typename T>
void Insertering(std::vector<T>& V) {
	for (typename std::vector<T>::iterator i = V.begin() + 1; i != V.end(); i++) {
		for (typename std::vector<T>::iterator j = i; j > V.begin() && *j < *(j - 1); j--)
		Swap(*j, *(j - 1));
	}
}


int main() {
	std::vector<int> Sempl{ 0, 12, -234, 1478, 332, -34534, 87 };
	Choosing(Sempl);
	for (int& i : Sempl) std::cout << i << ' ';
	return 0;
}
