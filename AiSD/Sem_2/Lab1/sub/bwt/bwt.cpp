#include "bwt.h"
#include <cstdint>
#include <string>

extern uint8_t num_byte;

std::string bwt_ns::bwt_1(const std::string& str, const uint8_t& num_byte) {
    if (str.size() > 1<<8) throw "err";
    std::string str_out;
    int size = str.size()/num_byte;
    std::vector<std::string> matrix(size);
    for (int i = 0; i < size; i++)
        for (int j = i; j < size + i; j++)
            matrix[i]+=str.substr((j%size)*num_byte, num_byte);
    std::sort(matrix.begin(), matrix.end());
    std::vector<std::string>::iterator it = std::find(matrix.begin(), matrix.end(), str);
    str_out+= uint8_t(it - matrix.begin());
    for (const std::string& s : matrix)
        str_out+=s.substr(s.size() - num_byte, num_byte);

#ifndef NDEBUG
    logger(log_ns::DEV_ONLY) << "str in "<<str<<std::endl;
    logger(log_ns::DEV_ONLY) << "str out "<<str_out<<" - " <<size_t(str_out.front())<<" : "<<(int)str_out.front()<<std::endl;
    logger(log_ns::DEV_ONLY  | log_ns::NORMAL_LVL) << "matrix: "<<std::endl;
    for (const std::string& s : matrix)
        logger(log_ns::DEV_ONLY  | log_ns::NORMAL_LVL) << s << std::endl;
#endif

    return str_out;
}

std::vector<std::string> bwt_ns::bwt_sort(const std::string& s_bwt, const uint8_t& pos_main) {
    uint8_t size = s_bwt.size() / num_byte;
    std::vector<std::string> v_pull(size, std::string(size, 0));
    std::map<std::string, uint8_t> map_bwt;
    for (int i = 0; i < size; i+=num_byte) map_bwt[s_bwt.substr(i, num_byte)]++;

    uint8_t pos = 0;
    for (std::pair<const std::string, uint8_t>& i : map_bwt)
        for (uint8_t j = 0; j < i.second; j++) {
            std::string& str = v_pull[pos];
            std::string::iterator startit = str.begin() + (size - 1) * num_byte;
            str.replace(startit, startit + num_byte, i.first);
            pos++;
        }


    for (uint8_t posx = 1; posx < size; posx++) {
        for (pos = 0; pos < size; pos++) {
            std::string& str = v_pull[pos];
            std::string::iterator startit = str.begin() + (size - 1 - posx) *num_byte;
            str.replace(startit, startit + num_byte, s_bwt.substr(pos, num_byte));
        }
        std::sort(v_pull.begin(), v_pull.end());
    }
    logger(log_ns::DEV_ONLY)<< "table BWT: "<< std::endl;
    for (const std::string str : v_pull)
        logger(log_ns::DEV_ONLY)<< str << std::endl;
    return v_pull;
}

std::string bwt_ns::de_bwt_1(const std::string& str, const uint8_t& num_byte) {
    bwt_ns::bwt_sort(str.substr(1, str.size() - 1), str[0]);
    return "";
}

void bwt(std::istream& stream_in, std::ostream& stream_out, const uint8_t& num_byte) {
    std::string buffer(1 << 8, 0);
    while (stream_in) {
        stream_in.read(buffer.data(), buffer.size());
        size_t read_bytes = stream_in.gcount();
        buffer.resize(read_bytes);
        std::string new_str = bwt_ns::bwt_1(buffer, num_byte);
        stream_out.write(new_str.data(),new_str.size());
    }
}
