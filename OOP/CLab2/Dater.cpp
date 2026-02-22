#include "Dater.h"

void House::add(IData* parent, const int& floors) {
    this->parent = parent;
    data[floors]++;
}

void House::printRepeating(std::ostream& os){
    for (cur = data.begin(); cur != data.end(); cur++){
        if (cur->second <= 1) continue;
        parent->printNow(os);
        os << cur->first << " встречается " << cur->second << " раз;" << std::endl;
    }

}

void Street::add(IData* parent, const int& house, const int& floors) {
    this->parent = parent;
    data[house].add(this, floors);
}

void Street::printRepeating(std::ostream& os){
    for (cur = data.begin(); cur != data.end(); cur++)
        cur->second.printRepeating(os);
}


void City::add(IData* parent, const std::string& street, const int& house, const int& floors) {
    this->parent = parent;
    data[street].add(this, house, floors);
    floorsMap[floors]++;
}

void City::printRepeating(std::ostream& os){
    for (cur = data.begin(); cur != data.end(); cur++)
        cur->second.printRepeating(os);
}

void City::printFloors(std::ostream& os) const {
    for(const std::pair<const int, int>& i : floorsMap){
        os << "\tДомов с этажами " << i.first<< ' ' << i.second << " штук;"<<std::endl;
    }
}

void Dater::add(const std::string& city, const std::string& street, const int& house, const int& floors) {
    data[city].add(this, street, house, floors);
}

void Dater::printRepeating(std::ostream& os) {
    for (cur = data.begin(); cur != data.end(); cur++)
        cur->second.printRepeating(os);
}

void Dater::printNow(std::ostream& os) {
    return;
}

void Dater::printFloors(std::ostream& os) const{
    for (const std::pair<const std::string, City>& i : data){
        os << i.first << ':' << std::endl;
        i.second.printFloors(os);
    }
}

#undef HM

