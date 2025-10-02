#include <iostream>
#include <vector>

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
		for (typename std::vector<T>::iterator j = i; j != V.end(); j++)
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
	Insertering(Sempl);
	for (int& i : Sempl) std::cout << i << ' ';
	return 0;
}