#include "lzw.h"
#include <cstdint>
#include <sstream>

using namespace lzw_ns;

std::ostream& lzw_ns::operator<<(std::ostream& os, const node& n) {
    os.write(reinterpret_cast<const char*>(&n.pos), n.size);
    return os;
}
std::istream& lzw_ns::operator>>(std::istream& is, node& n) {
    is.read(reinterpret_cast<char*>(&n.pos), n.size);
    return is;
}

std::string lzw_ns::lzw_0(const std::string& str, size_t buffer_size, const uint8_t& num_byte, std::vector<std::string>& v_slovar) {
    if (num_byte > 1) throw "hz";
    std::stringstream ss;
    for(size_t i = 0; i < str.size() - num_byte; ) {
        node n;
        std::vector<std::string>::iterator it = std::find(v_slovar.begin(), v_slovar.end(), std::string_view(str.data() + i, 1));
        for (size_t j = i + num_byte * 2; j < std::min(i + buffer_size, str.size()); j+=num_byte) {
            logger(log_ns::DEV_ONLY | log_ns::HARD_LVL)<<"searc "<<std::string_view(str.data() + i, j - i)<<std::endl;
            std::vector<std::string>::iterator it1 = std::find(it, v_slovar.end(), std::string_view(str.data() + i, j - i));
            if(it1 == v_slovar.end())
                break;
            if(it->size() < it1->size())
                it = it1;
            logger(log_ns::DEV_ONLY | log_ns::HARD_LVL)<<"find "<<*it<<std::endl;
        }
        if (it == v_slovar.end()) throw "ERR";
        n.pos = it - v_slovar.begin();
        size_t size_tmp = it->size();
        logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL) <<"to file "<<n.pos<<std::endl;
        if(v_slovar.size() > 2 << (node::size * 8))
            node::size++;
        logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL) << "find " << *it;
        i+=size_tmp;
        v_slovar.push_back(*it + str[i]);
        logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL) <<" to slovar "<<v_slovar.back()<<std::endl;
        ss<<n;
    }
    logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<"slovar extend: "<<std::endl;
    for (size_t i = 2<<8; i < v_slovar.size(); i++)
        logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<v_slovar[i]<<std::endl;
    return ss.str();
}

std::string lzw_ns::de_lzw_0(const std::string& str, size_t buffer_size, const uint8_t& num_byte, std::vector<std::string>& v_slovar) {
    std::stringstream ss;
    ss<<str;
    node n;
    std::string str_out, last;
    for(size_t i = 0; i < str.size() - num_byte; ) {
        ss >> n;
        logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL) << "read "<< v_slovar[n.pos]<<std::endl;
        if (last.size()) {
            v_slovar.push_back(last + v_slovar[n.pos].front());
            logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL) << "to slovar " <<v_slovar.back() <<std::endl;
            if(v_slovar.size() > 2 << (node::size * 8))
                node::size++;
        }
        last = v_slovar[n.pos];
        str_out+=last;
    }

    return str_out;
}

std::string lzw_ns::lzw_1(const std::string& str, size_t buffer_size, const uint8_t& num_byte) {
    std::vector<std::string> v_slovar(2<<8);
    // std::iota(v_slovar.begin(), v_slovar.end(), 0);
    for (size_t i = 0; i < 2<<8; i++)
        v_slovar[i] = uint8_t(i);
    return lzw_0(str,buffer_size,num_byte,v_slovar);
}

std::string de_lzw_1(const std::string& str, size_t buffer_size, const uint8_t& num_byte) {
    std::vector<std::string> v_slovar(2<<8);
    for (size_t i = 0; i < 2<<8; i++)
        v_slovar[i] = uint8_t(i);
    return de_lzw_0(str,buffer_size,num_byte,v_slovar);
}
