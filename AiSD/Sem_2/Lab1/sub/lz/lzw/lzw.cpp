#include "lzw.h"

using namespace lzw_ns;

std::ostream& operator<<(std::ostream& os, const node& n) {
    os.write(reinterpret_cast<const char*>(&n.pos), 2);
    os.write(reinterpret_cast<const char*>(&n.next), 2);
    return os;
}
std::istream& operator>>(std::istream& is, node& n);

std::string lzw_1(const std::string& str, size_t buffer_size, const uint8_t& num_byte) {
    if (num_byte > 1) throw "hz";

    std::stringstream ss;
    std::vector<std::string> v_slovar(1, "");
    for(size_t i = 0; i < str.size() - num_byte; i+=num_byte) {
        node n;
        std::vector<std::string>::iterator it = v_slovar.begin();
        for (size_t j = i + num_byte; j < std::min(i + buffer_size, str.size()); j+=num_byte) {
            logger(log_ns::DEV_ONLY | log_ns::HARD_LVL)<<"searc "<<std::string_view(str.data() + i, j - i)<<std::endl;
            std::vector<std::string>::iterator it1 = std::find(it, v_slovar.end(), std::string_view(str.data() + i, j - i));
            if(it1 == v_slovar.end())
                break;
            if(it->size() < it1->size())
                it = it1;
            logger(log_ns::DEV_ONLY | log_ns::HARD_LVL)<<"find "<<*it1<<std::endl;
        }
        logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL) << "find " << *it << " push to slovar "<<v_slovar.back()<<std::endl;
        n.pos = it - v_slovar.begin();
        n.next = std::string_view(str.data() + i + it->size(), num_byte);
        ss<<n;
        i+=it->size();
        if (v_slovar.size() < 256)
            v_slovar.push_back(*it + n.next);
        logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL) <<"to file "<<n.pos<<' '<<n.next<<std::endl;
        if(v_slovar.size() > 256) throw"ERR";
    }
    logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<"slovar: "<<std::endl;
    for (const std::string& s :v_slovar)
        logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<s<<std::endl;
    return ss.str();
}

std::string de_lzw_1(const std::string& str, size_t buffer_size, const uint8_t& num_byte);
