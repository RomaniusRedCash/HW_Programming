#include "Dater.h"

void house::add(const int& floors) {
    data[floors]++;
}

std::vector<int> house::printRepeating(const int& i)
{
    for
}


void Street::add(const int& house, const int& floors) {
    data[house].add(floors);
}

//HM<int, house> City::operator[] (const std::wstring& city) { return subData[city]; }

//City Dater::operator[](const std::wstring& city) {
//    return data[city];
//}

void City::add(const std::wstring& street, const int& house, const int& floors) {
    data[street].add(house, floors);
    floorsMap[floors]++;
}

void Dater::add(const std::wstring& city, const std::wstring& street, const int& house, const int& floors) {
    data[city].add(street, house, floors);
}


#undef HM

