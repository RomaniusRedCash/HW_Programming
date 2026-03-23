#include "lz78.h"

using namespace lz78_ns;

std::ostream& lz78_ns::operator<<(std::ostream& os, const node& n) {
    os.write(reinterpret_cast<const char*>(&n.pos), 1);
    // os.write(reinterpret_cast<const char*>(&n.len), 1);
    os.write(n.next.data(), n.next.size());
    return os;
}

std::istream& lz78_ns::operator>>(std::istream& is, node& n) {
    is.read(reinterpret_cast<char*>(&n.pos), 1);
    // is.read(reinterpret_cast<char*>(&n.len), 1);
    is.read(n.next.data(), n.next.size());
    return is;
}

std::string lz78_ns::lz78_1(const std::string& str, size_t buffer_size, const uint8_t& num_byte) {
    std::stringstream ss;
    std::vector<std::string> v_slovar(1, "");
    for(size_t i = 0; i < str.size() - num_byte; i+=num_byte) {
        node n;
        std::vector<std::string>::iterator it = v_slovar.begin();
        for (size_t j = i + num_byte; j < std::min(i + buffer_size, str.size()); j+=num_byte) {
            std::vector<std::string>::iterator it1 = std::find(v_slovar.begin(), v_slovar.end(), std::string_view(str.data() + i, j - i));
            if(it1 == v_slovar.end())
                break;
            it = it1;
            // n.pos= v_slovar.end() - it + 1;
        }
        // if (it == v_slovar.end()) it = v_slovar.begin();
        n.pos = it - v_slovar.begin() + 1;
        // n.pos++;
        n.next = std::string_view(str.data() + i + it->size(), num_byte);
        ss<<n;
        v_slovar.push_back(*it + n.next);
#ifdef DEBUG
        logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL) << "find " << *it << " push to slovar "<<v_slovar.back()<<std::endl;
        logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL) <<"to file "<<n.pos<<' '<<n.next<<std::endl;
#endif
        if(v_slovar.size() > 256) throw"ERR";
    }
    return ss.str();
}

std::string lz78_ns::de_lz78_1(const std::string& str, size_t buffer_size, const uint8_t& num_byte) {
    std::vector<std::string> v_slovar(1, "");
    std::stringstream ss;
    std::string str_out;
    node n;
    n.next.resize(num_byte);
    for (size_t i = 0; i < str.size(); i+=num_byte+1) {
        ss>>n;
        v_slovar.push_back(v_slovar[n.pos] + n.next);
        str_out += v_slovar.back();
    }
    return str_out;
}

