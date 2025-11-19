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



#undef HM

