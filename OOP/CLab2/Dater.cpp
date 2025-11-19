#include "Dater.h"

void house::add(IData* parent, const int& floors) {
    this->parent = parent;
    data[floors]++;
}

void house::printRepeating(std::wostream& os){
    for (cur = data.begin(); cur != data.end(); cur++){
        if (cur->second <= 1) continue;
        parent->printNow(os);
        os << cur->first << L" встречается " << cur->second << L" раз;" << std::endl;
    }

}

void Street::add(IData* parent, const int& house, const int& floors) {
    this->parent = parent;
    data[house].add(this, floors);
}

void Street::printRepeating(std::wostream& os){
    for (cur = data.begin(); cur != data.end(); cur++)
        cur->second.printRepeating(os);
}


void City::add(IData* parent, const std::wstring& street, const int& house, const int& floors) {
    this->parent = parent;
    data[street].add(this, house, floors);
    floorsMap[floors]++;
}

void City::printRepeating(std::wostream& os){
    for (cur = data.begin(); cur != data.end(); cur++)
        cur->second.printRepeating(os);
}

void City::printFloors(std::wostream& os) const {
    for(const std::pair<const int, int>& i : floorsMap){
        os << L"\tДомов с этажами " << i.first<< L' ' << i.second << L" штук;"<<std::endl;
    }
}

void Dater::add(const std::wstring& city, const std::wstring& street, const int& house, const int& floors) {
    data[city].add(this, street, house, floors);
}

void Dater::printRepeating(std::wostream& os) {
    for (cur = data.begin(); cur != data.end(); cur++)
        cur->second.printRepeating(os);
}

void Dater::printNow(std::wostream& os) {
    return;
}

void Dater::printFloors(std::wostream& os) const{
    for (const std::pair<const std::wstring, City>& i : data){
        os << i.first << L':' << std::endl;
        i.second.printFloors(os);
    }
}

#undef HM

