#include "lz77.h"

using namespace lz77_ns;

std::ostream& lz77_ns::operator<<(std::ostream& os, const node& n) {
    os.write(reinterpret_cast<const char*>(&n.pos), 1);
    os.write(reinterpret_cast<const char*>(&n.len), 1);
    os.write(n.next.data(), n.next.size());
    return os;
}

std::istream& lz77_ns::operator>>(std::istream& is, node& n) {
    is.read(reinterpret_cast<char*>(&n.pos), 1);
    is.read(reinterpret_cast<char*>(&n.len), 1);
    is.read(n.next.data(), n.next.size());
    return is;
}

std::string lz77_ns::lz77_1(const std::string& str, size_t buffer_size, const uint8_t& num_byte) {
    buffer_size *= num_byte;
    std::stringstream ss;
    if (buffer_size > 256) throw "ERR";
    for (size_t i = 0; i < str.size(); i+=num_byte) {
        node n1;
        size_t start = 0;
        if (i >= buffer_size)
            start = i - buffer_size;
        std::string_view str_vi(str.data() + start, std::min(buffer_size, i));
        logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<"str_vi is "<<str_vi<<std::endl;
        for (size_t j = i + num_byte; j != std::min(i + buffer_size, str.size()) && j - i + num_byte <= str_vi.size(); j+=num_byte){
            logger(log_ns::DEV_ONLY | log_ns::HARD_LVL) << "search "<<std::string_view(str.data() + i, j - i)<<std::endl;
            size_t tmp_pos = str_vi.find(std::string_view(str.data() + i, j - i), (n1.pos == 0 ? 0 :str_vi.size() - n1.pos));
            if (tmp_pos == str_vi.npos) {
                logger(log_ns::DEV_ONLY | log_ns::HARD_LVL) <<"not found. last is "<<n1.pos<<' '<<n1.len<<std::endl;
                break;
            }
            logger(log_ns::DEV_ONLY | log_ns::HARD_LVL) << "tmp_pos is "<<tmp_pos<<std::endl;
            n1.pos = str_vi.size() - tmp_pos;
            n1.len++;
        }
        n1.pos /= num_byte;
        n1.next = std::string_view(str.data() + i + n1.len,num_byte);
        i+=n1.len*num_byte;
#ifndef NDEBUG
        if(n1.len) {
            logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<"found "<<std::string_view(str.data()+n1.pos,n1.len)<<std::endl;
        }
        logger(log_ns::DEV_ONLY| log_ns::NORMAL_LVL)<<"pos "<<n1.pos<<" len "<<n1.len<<' '<<n1.next<<std::endl;
#endif
        ss<<n1;
    }
    return ss.str();
}

std::string lz77_ns::de_lz77_1(const std::string& str, size_t buffer_size, const uint8_t& num_byte) {
    std::stringstream is;
    is << str;
    std::string ss;
    std::string_view buffer;
    node n;
    n.next.resize(num_byte);
    for (size_t i = 0; i < str.size(); i+=2 + num_byte) {
        is >> n;
        logger(log_ns::DEV_ONLY)<<"pos "<<n.pos<<" len "<<n.len<<' '<<n.next<<std::endl;
        ss += std::string_view(buffer.data() + buffer.size() - n.pos, n.len);
        ss += n.next;
        if (ss.size() >= buffer_size)
            buffer = std::string_view(ss.data() + ss.size() -  buffer_size, buffer_size);
        else
            buffer = std::string_view(ss.data(), ss.size());
    }
    return ss;
}

