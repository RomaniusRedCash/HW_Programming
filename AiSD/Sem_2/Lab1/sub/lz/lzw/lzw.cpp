#include "lzw.h"

using namespace lzw_ns;

std::ostream& lzw_ns::operator<<(std::ostream& os, const node& n) {
    logger(log_ns::DEV_ONLY | log_ns::HARD_LVL)<<"write "<<n.pos<<" with size "<<size_t(n.size)<<std::endl;
    os.write(reinterpret_cast<const char*>(&n.pos), n.size);
    return os;
}
std::istream& lzw_ns::operator>>(std::istream& is, node& n) {
    n.pos = 0;
    std::string buffer(n.size, 0);
    is.read(buffer.data(), n.size);
    if (!is)
        logger() << "WARN read "<<is.gcount()<<" from "<<size_t(n.size)<<std::endl;
    for (size_t i = 0; i < n.size; ++i) {
        uint8_t byte = static_cast<uint8_t>(buffer[i]);
        n.pos |= static_cast<size_t>(byte) << (i * 8);
    }
    return is;
}

std::string lzw_ns::lzw_0(const std::string& str, size_t buffer_size, const uint8_t& num_byte, std::vector<std::string>& v_slovar) {
    size_t slov_szie_start = v_slovar.size();
    if (num_byte > 1) throw "hz";
    std::stringstream ss;
    node n;
    n.size = num_byte;
    for(size_t i = 0; i < str.size() - num_byte; ) {
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
        // logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL) <<"to file "<<n.pos<<std::endl;
        if(v_slovar.size() > 2 << (node::size * 8))
            node::size++;
        logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL) << "find " << *it;
        i+=size_tmp;
        v_slovar.push_back(*it + str[i]);
        logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL) <<" to slovar "<<v_slovar.back()<<std::endl;
        ss<<n;
    }
    n.pos = (std::find(v_slovar.begin(), v_slovar.end(), std::string_view(str.data() + str.size() - 1, num_byte)) - v_slovar.begin());
    ss<<n;
    logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<"slovar extend: "<<std::endl;
    for (size_t i = slov_szie_start; i < v_slovar.size(); i++)
        logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<v_slovar[i]<<std::endl;
    return ss.str();
}

std::string lzw_ns::de_lzw_0(const std::string& str, size_t buffer_size, const uint8_t& num_byte, std::vector<std::string>& v_slovar) {
    std::stringstream ss;
    ss<<str;
    node n;
    n.size = 1;
    ss>>n;
    std::string str_out, last;
    last = v_slovar[n.pos];
    str_out+=last;
    for(size_t i = n.size; i < str.size(); i+=n.size) {
        if(v_slovar.size() + 1 > 2 << (node::size * 8))
            node::size++;
        ss >> n;
        logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL) << "read "<<n.pos<<" - "<< v_slovar[n.pos]<<' ';
        v_slovar.push_back(last + v_slovar[n.pos].front());
        logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL) << "to slovar " <<v_slovar.back() <<std::endl;
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

std::string lzw_ns::lzw_2(const std::string& str, size_t buffer_size, const uint8_t& num_byte) {
    std::set<std::string> s_slovar;
    for (size_t i = 0; i < str.size(); i+=num_byte) s_slovar.insert(str.substr(i, num_byte));
    std::vector<std::string> v_slovar(s_slovar.begin(), s_slovar.end());
    size_t size = v_slovar.size();
    std::string str_out;
    str_out.reserve(sizeof(size) + size * num_byte);
    str_out.resize(sizeof(size));
    std::memcpy(str_out.data(), &size, sizeof(size));

    for (const std::string& s : v_slovar)
        str_out+=s;
    return str_out+=lzw_0(str,buffer_size,num_byte,v_slovar);
}

std::string lzw_ns::de_lzw_1(const std::string& str, size_t buffer_size, const uint8_t& num_byte) {
    std::vector<std::string> v_slovar(2<<8);
    for (size_t i = 0; i < 2<<8; i++)
        v_slovar[i] = uint8_t(i);
    return de_lzw_0(str,buffer_size,num_byte,v_slovar);
}

std::string lzw_ns::de_lzw_2(const std::string& str, size_t buffer_size, const uint8_t& num_byte) {
    size_t size;
    std::memcpy(&size, str.data(), sizeof(size));
    std::vector<std::string> v_slovar(size, std::string(num_byte, 0));
    for (size_t i = 0; i < size * num_byte; i +=num_byte)
        v_slovar[i] = std::string_view(str.data() + sizeof(size) + i * num_byte, num_byte);
    std::string str_in(str.begin() + sizeof(size) + size * num_byte, str.end());
    return de_lzw_0(str_in,buffer_size,num_byte,v_slovar);
}
