#include "bwt.h"
#include <algorithm>

std::string bwt_ns::bwt_1(const std::string& str, const uint8_t& num_byte) {
    std::string str_out;
    int size = str.size()/num_byte;
    std::vector<std::string> matrix(size);
    for (int i = 0; i < size; i++)
        for (int j = i; j < size + i; j++)
            matrix[i]+=str.substr((j%size)*num_byte, num_byte);
    std::sort(matrix.begin(), matrix.end());
    std::vector<std::string>::iterator it = std::find(matrix.begin(), matrix.end(), str);
    for (const std::string& s :matrix)
        str_out+=s.back();

    str_out.insert(str_out.begin(), uint8_t(matrix.end() - it));

#ifdef DEBUG
    logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL) << "str in "<<str<<std::endl;
    logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL) << "str out "<<str_out<<" - " <<size_t(str_out.front())<<" : "<<str_out.substr(1)<<std::endl;
#endif

    return str_out;
}

void bwt(std::istream& stream_in, std::ostream& stream_out, const uint8_t& num_byte) {
    std::string buffer(256, 0);
    while (stream_in) {
        stream_in.read(buffer.data(), buffer.size());
        size_t read_bytes = stream_in.gcount();
        if (read_bytes != buffer.size()) buffer.resize(read_bytes);
    }
}
