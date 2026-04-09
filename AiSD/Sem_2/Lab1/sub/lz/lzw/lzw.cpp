#include "lzw.h"
#include <cstring>

using namespace lzw_ns;

std::vector<std::string> lzw_ns::create_slovar_byte(const uint8_t& num_byte) {
    std::vector<std::string> v_slovar(1<<(8 * num_byte), std::string(num_byte, 0));
    for (size_t i = 0; i < 1<<(8 * num_byte); i++)
        std::memcpy(v_slovar[i].data(), &i, num_byte);
    return v_slovar;
}

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

std::string lzw_ns::lzw_0(const std::string& str, size_t buffer_size, const uint8_t& num_byte, std::vector<std::string> v_slovar) {
    logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL) << "chunk size "<<str.size()<<" buffer size "<<buffer_size<<" num_byte "<< size_t(num_byte)<<std::endl;
    size_t slov_szie_start = v_slovar.size();
    if (num_byte > 1) throw "hz";
    std::stringstream ss;
    node n;
    n.size = num_byte;
    std::string last;
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
        if(v_slovar.size() > 1 << (node::size * 8))
            node::size++;
        logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL) << "find " << *it;
        i+=size_tmp;
        if (last.size()) {
            v_slovar.push_back(last + *it);
            logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL) <<" to slovar "<<v_slovar.back();
        }
        logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL) << std::endl;
        last = *it;
        ss<<n;
    }
    n.pos = (std::find(v_slovar.begin(), v_slovar.end(), std::string_view(str.data() + str.size() - 1, num_byte)) - v_slovar.begin());
    ss<<n;
    logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<"slovar extend: "<<std::endl;
    for (size_t i = slov_szie_start; i < v_slovar.size(); i++)
        logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<v_slovar[i]<<std::endl;
    return ss.str();
}

std::string lzw_ns::de_lzw_0(const std::string& str, size_t buffer_size, const uint8_t& num_byte, std::vector<std::string> v_slovar) {
    std::stringstream ss;
    ss<<str;
    node n;
    n.size = 1;
    ss>>n;
    std::string str_out, last;
    last = v_slovar[n.pos];
    str_out+=last;
    for(size_t i = n.size; i < str.size(); i+=n.size) {
        if(v_slovar.size() + 1 > 1 << (node::size * 8))
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
    return lzw_0(str,buffer_size,num_byte, create_slovar_byte(num_byte));
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
    return de_lzw_0(str,buffer_size,num_byte, create_slovar_byte(num_byte));
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

#define BUFFER_SIZE 1024 * num_byte
void lzw(std::istream& stream_in, std::ostream& stream_out, const uint8_t& num_byte, const size_t& buffer_size) {
    std::string buffer(BUFFER_SIZE, 0);
    while (!stream_in.eof()) {
        stream_in.read(buffer.data(), BUFFER_SIZE);
        size_t read_bites = stream_in.gcount();
        buffer.resize(read_bites);
        std::string new_buffer = lzw_1(buffer, buffer_size, num_byte);
        size_t size = new_buffer.size() / num_byte;
        stream_out.write(reinterpret_cast<const char*>(&size), sizeof(size));
        stream_out.write(new_buffer.data(), size);
        if (read_bites <BUFFER_SIZE) break;
    }
}
void de_lzw(std::istream& stream_in, std::ostream& stream_out, const uint8_t& num_byte, const size_t& buffer_size) {
    std::string buffer(BUFFER_SIZE, 0);
    while (!stream_in.eof()) {
        size_t size = 0;
        stream_in.read(reinterpret_cast<char*>(&size), sizeof(size));
        size*=num_byte;
        stream_in.read(buffer.data(), size);
        buffer.resize(size);
        size_t read_bites = stream_in.gcount();
        if (read_bites < size) break;
        std::string new_buffer = de_lzw_1(buffer, buffer_size, num_byte);
        stream_out.write(new_buffer.data(), new_buffer.size());
    }
}
#undef BUFFER_SIZE
