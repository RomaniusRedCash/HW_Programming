#include "bwt.h"
#include <cstdint>

extern uint8_t num_byte;

using namespace bwt_ns;

std::string bwt_ns::bwt_1(const std::string& str) {
    if (str.size() > 1<<8) throw "err";
    std::string str_out;
    size_t size = str.size()/num_byte;
    std::vector<std::string> matrix(size);
    for (uint8_t i = 0; i < size; i++)
        for (uint8_t j = i; j < size + i; j++)
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

std::string bwt_ns::de_bwt_1(const std::string& s_bwt, const uint8_t& bwt_pos) {
    size_t size = s_bwt.size() / num_byte;
    std::vector<std::string> v_pull(size, std::string(size, 0));
    std::map<std::string, uint8_t> map_bwt;
    for (int i = 0; i < size; i+=num_byte) map_bwt[s_bwt.substr(i, num_byte)]++;

    size_t pos = 0;
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
            std::string::iterator startit = str.begin() + (size - 1 - posx) * num_byte;
            str.replace(startit, startit + num_byte, std::string_view(s_bwt.data() + pos * num_byte, num_byte));
        }
        std::sort(v_pull.begin(), v_pull.end());
    }
    logger(log_ns::DEV_ONLY)<< "table BWT: "<< std::endl;
    for (const std::string str : v_pull)
        logger(log_ns::DEV_ONLY)<< str << std::endl;
    logger(log_ns::DEV_ONLY)<< "====" << std::endl;
    return v_pull[bwt_pos];
}

void bwt_ns::bwt_sort_good(std::vector<std::string>*& pv_pull1, std::vector<std::string>*& pv_pull2, const std::vector<uint8_t>& v_subpull) {
    for (std::vector<uint8_t>::const_iterator it = v_subpull.begin(); it != v_subpull.end(); it++)
        (*pv_pull2)[it - v_subpull.begin()] = (*pv_pull1)[*it];
    std::swap(pv_pull1, pv_pull2);
}

std::string bwt_ns::de_bwt_2(const std::string& s_bwt, const uint8_t& bwt_pos) {
    size_t size = s_bwt.size() / num_byte;
    std::vector<std::string> v_pull1(size, std::string(size, 0)), *pv_pull1 = &v_pull1;
    std::vector<std::string> v_pull2(size, std::string(size, 0)), *pv_pull2 = &v_pull2;
    std::map<std::string, uint8_t> map_bwt;
    for (size_t i = 0; i < size; i+=num_byte) map_bwt[s_bwt.substr(i, num_byte)]++;
    size_t pos = 0;
    for (std::pair<const std::string, uint8_t>& i : map_bwt)
        for (uint8_t j = 0; j < i.second; j++) {
            std::string& str = v_pull1[pos];
            std::string::iterator startit = str.begin() + (size - 1) * num_byte;
            str.replace(startit, startit + num_byte, i.first);
            str.replace(startit - num_byte, startit, std::string_view(s_bwt.data() + pos * num_byte, num_byte));
            pos++;
        }
    std::vector<uint8_t> v_subpull(size);
    pos = 0;
    for (uint8_t& i : v_subpull) {
        i = pos++;
    }

#ifndef NDEBUG
    logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<< "table BWT: "<< std::endl;
    for (const std::string str : *pv_pull1)
        logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<< str << std::endl;
    logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<< "====" << std::endl;
#endif

    std::sort(v_subpull.begin(), v_subpull.end(), [&v_pull1](const uint8_t& a, const uint8_t& b){return v_pull1[a] < v_pull1[b]; });

#ifndef NDEBUG
    pos = 0;
    logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL) << "perenos:"<<std::endl;
    for (const uint8_t& i : v_subpull) logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL) << int(i) << " -> " << int(pos++) << std::endl;
#endif

    bwt_sort_good(pv_pull1, pv_pull2, v_subpull);

#ifndef NDEBUG
    logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<< "table BWT: "<< std::endl;
    for (const std::string str : *pv_pull1)
        logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<< str << std::endl;
    logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<< "====" << std::endl;
#endif

    for (uint8_t posx = 2; posx < size; posx++) {
        for (pos = 0; pos < size; pos++) {
            std::string& str = (*pv_pull1)[pos];
            std::string::iterator startit = str.begin() + (size - 1 - posx) * num_byte;
            str.replace(startit, startit + num_byte, std::string_view(s_bwt.data() + pos * num_byte, num_byte));
        }
        bwt_sort_good(pv_pull1, pv_pull2, v_subpull);

#ifndef NDEBUG
        logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<< "table BWT: "<< std::endl;
        for (const std::string str : *pv_pull1)
            logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<< str << std::endl;
        logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<< "====" << std::endl;
#endif
    }
#ifndef NDEBUG
    logger(log_ns::DEV_ONLY)<< "table BWT: "<< std::endl;
    for (const std::string str : *pv_pull1)
        logger(log_ns::DEV_ONLY)<< str << std::endl;
    logger(log_ns::DEV_ONLY)<< "====" << std::endl;
#endif
    return (*pv_pull1)[bwt_pos];
}

std::string bwt_ns::de_bwt_3(const std::string& s_bwt, const size_t& bwt_pos) {
    const size_t size = s_bwt.size() / num_byte;
    if (size >= 1 << sizeof(uint8_t) * 8) throw "ERR";
    std::unordered_map<std::string_view, std::deque<std::string_view>> umapa;
    std::map<std::string_view, uint8_t> mapa;
    for (uint8_t i = 0; i < size; i++)
        mapa[{s_bwt.data() + i*num_byte, num_byte}]++;
    size_t pos = 0;
    for (std::pair<const std::string_view, uint8_t>& i : mapa)
        for (uint8_t j = 0; j < i.second; j++)
            umapa[i.first].emplace_back(s_bwt.data() + pos++ * num_byte, num_byte);

#ifndef NDEBUG
    logger(log_ns::DEV_ONLY) << "waymap:"<<std::endl;
    for (std::pair<const std::string_view, std::deque<std::string_view>>& pr : umapa) {
        logger(log_ns::DEV_ONLY) << pr.first << " - ";
        for (const std::string_view& str : pr.second)
            logger(log_ns::DEV_ONLY) << str<<' ';
        logger(log_ns::DEV_ONLY) << std::endl;
    }
#endif

    std::string str_out(size*num_byte, 0);
    str_out.replace((size - 1)*num_byte, num_byte, s_bwt.substr(bwt_pos*num_byte, num_byte));
    pos = 1;
    while(pos <size) {
        std::deque<std::string_view>& dqu = umapa[{str_out.data() + (size - pos++) * num_byte, num_byte}];
        if (dqu.empty()) break;
        str_out.replace((size - pos)*num_byte, num_byte, dqu.front());
        dqu.pop_front();
    }
    return str_out;
}

std::string bwt_ns::de_bwt_0(const std::string& str) {
    return bwt_ns::de_bwt_3(str.substr(1, str.size() - 1), str[0]);
}

void bwt(std::istream& stream_in, std::ostream& stream_out) {
    std::string buffer(1 << (sizeof(uint8_t) * 8), 0);
    while (stream_in) {
        stream_in.read(buffer.data(), buffer.size());
        buffer.resize(stream_in.gcount());
        if (buffer.empty()) return;
        std::string new_str = bwt_ns::bwt_1(buffer);
        stream_out.write(new_str.data(),new_str.size());
    }
}
void de_bwt(std::istream& stream_in, std::ostream& stream_out) {
    std::string buffer((1 << (sizeof(uint8_t) * 8)) + 1, 0);
    while (stream_in) {
        stream_in.read(buffer.data(), buffer.size());
        buffer.resize(stream_in.gcount());
        if (buffer.empty()) return;
        std::string new_str = bwt_ns::de_bwt_0(buffer);
        stream_out.write(new_str.data(),new_str.size());
    }
}
