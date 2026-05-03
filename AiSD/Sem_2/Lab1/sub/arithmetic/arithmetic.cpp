#include "arithmetic.h"
#include <iostream>


void ar_nc::maker(std::unordered_map<char, std::pair<size_t*, size_t>>& m_slovar, std::vector<std::pair<const char, std::pair<size_t*, size_t>>*>& v_slovar, const size_t& str_size) {
    static size_t zero = 0;
    std::for_each(m_slovar.begin(), m_slovar.end(), [&v_slovar](std::pair<const char, std::pair<size_t*, size_t>>& c){v_slovar.push_back(&c);});
    std::sort(v_slovar.begin(), v_slovar.end(), [](std::pair<const char, std::pair<size_t*, size_t>>*& a, std::pair<const char, std::pair<size_t*, size_t>>*& b){ return a->second.second > b->second.second;});
    std::for_each(v_slovar.begin(), v_slovar.end(), [](std::pair<const char, std::pair<size_t*, size_t>>*& a){
        std::cout<<a->first<<" : {" << 0<<", "<<a->second.second<<"} ";
    });
    std::cout<<std::endl;
    v_slovar.front()->second.first = &zero;
    for (std::vector<std::pair<const char, std::pair<size_t*, size_t>>*>::iterator it = v_slovar.begin() + 1; it != v_slovar.end(); it++) {
        (*it)->second.first = &(*(it - 1))->second.second;
        (*it)->second.second += *(*it)->second.first;
    }
    std::for_each(v_slovar.begin(), v_slovar.end(), [](std::pair<const char, std::pair<size_t*, size_t>>*& a){
        std::cout<<a->first<<" : {" << *a->second.first<<", "<<a->second.second<<"} ";
    });
    std::cout<<std::endl;
}

std::stringstream arithmetic(const std::string& str_in) {
    std::vector<std::pair<const char, std::pair<size_t*, size_t>>*> v_slovar;
    std::stringstream ss;
    size_t out, len = str_in.size();
    std::unordered_map<char, std::pair<size_t*, size_t>> m_slovar;
    std::for_each(str_in.begin(), str_in.end(), [&m_slovar](const char& c){m_slovar[c].second++;});
    size_t size_m_slov = m_slovar.size();
    ss.write(reinterpret_cast<const char*>(&size_m_slov), sizeof(size_m_slov));
    std::for_each(m_slovar.begin(), m_slovar.end(), [&ss](std::pair<const char, std::pair<size_t*, size_t>>& c){
        ss.write(reinterpret_cast<const char*>(&c.first), sizeof(c.first));
        ss.write(reinterpret_cast<const char*>(&c.second.second), sizeof(c.second.second));});
    size_t top = std::numeric_limits<size_t>::max();
    size_t buttom = 0;
    ar_nc::maker(m_slovar, v_slovar, len);
    std::for_each(str_in.begin(), str_in.end(), [&](const char& c){
        unsigned __int128 range = (unsigned __int128)top - buttom;
        size_t prev_buttom = buttom;
        top = prev_buttom + (size_t)(range * m_slovar[c].second / len);
        buttom = prev_buttom + (size_t)(range * (*m_slovar[c].first) / len);
    });
    size_t pos = buttom + (top - buttom) / 2;
    std::cout<<pos<<std::endl;
    ss.write(reinterpret_cast<const char*>(&len), sizeof(len));
    ss.write(reinterpret_cast<const char*>(&pos), sizeof(pos));
    ss.clear();
    ss.seekg(0, std::ios::beg);
    return ss;
}
std::string de_arithmetic(std::stringstream& ss_in) {
    std::vector<std::pair<const char, std::pair<size_t*, size_t>>*> v_slovar;
    std::string str_out;
    size_t zero = 0;
    std::unordered_map<char, std::pair<size_t*, size_t>> m_slovar;
    size_t slov_size;
    ss_in.read(reinterpret_cast<char*>(&slov_size), sizeof(slov_size));
    for (size_t i = 0; i < slov_size; i++) {
        char c; size_t end;
        ss_in.read(reinterpret_cast<char*>(&c), sizeof(c));
        ss_in.read(reinterpret_cast<char*>(&end), sizeof(end));
        m_slovar[c].second = end;
    }
    ss_in.read(reinterpret_cast<char*>(&slov_size), sizeof(slov_size));
    ar_nc::maker(m_slovar, v_slovar, slov_size);
    size_t top = std::numeric_limits<size_t>::max();
    size_t buttom = 0;
    size_t data_st;
    ss_in.read(reinterpret_cast<char*>(&data_st), sizeof(data_st));
    size_t current_low = 0;
    size_t current_range = std::numeric_limits<size_t>::max();

    for (size_t i = 0; i < slov_size; i++) {
        size_t range = top - buttom;
        auto it = std::find_if(v_slovar.begin(), v_slovar.end(), [&](auto& a) {
            size_t sym_low = buttom + (size_t)((unsigned __int128)range * *a->second.first / slov_size);
            size_t sym_high = buttom + (size_t)((unsigned __int128)range * a->second.second / slov_size);

            return data_st >= sym_low && data_st < sym_high;
        });

        if (it != v_slovar.end()) {
            str_out += (*it)->first;
            size_t low_cum = *(*it)->second.first;
            size_t high_cum = (*it)->second.second;
            size_t next_top = buttom + (size_t)((unsigned __int128)range * high_cum / slov_size);
            buttom = buttom + (size_t)((unsigned __int128)range * low_cum / slov_size);
            top = next_top;
        }
    }
    return str_out;
}
