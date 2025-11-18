#include "Dater.h"

HM<int, HomeStr> City::operator[] (const std::wstring& city) { return subData[city]; }

City Dater::operator[](const std::wstring& city) {
    return data[city];
}

void Dater::add(const std::wstring& city, const std::wstring& street, const int& house, const int& floors) {
    HomeStr& houseStr = data[city][street][house];
    if (houseStr.repeats == 0)
        houseStr.floors = floors;
    
    //else if (houseStr.floors != floors) throw u8"Два дома с одинаковым адресом";
    houseStr.repeats++;
    data[city].floorsMap[floors]++;
}

#undef HM
