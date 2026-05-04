#include "lzw.h"

using namespace lzw_ns;

std::vector<std::string> lzw_ns::create_slovar_byte(const size_t& start, size_t const& stop) {
    std::vector<std::string> v_slovar;
    v_slovar.reserve(1<<(8 * num_byte));
    for (size_t i = start; i < stop; i++){
        std::string str(num_byte, 0);
        std::memcpy(str.data(), &i, num_byte);
        v_slovar.push_back(str);
    }
    return v_slovar;
}

std::unordered_map<std::string, size_t> lzw_ns::create_mapslovar_byte(const size_t& start, size_t const& stop) {
    std::unordered_map<std::string, size_t> map;
    for (size_t i = start; i < stop; i++) {
        std::string str(num_byte, 0);
        std::memcpy(str.data(), &i, num_byte);
        map[str] = i - start;
    }
    return map;
}

sstrtobb& lzw_ns::operator<<(sstrtobb& os, node n) {
    bytebit bb(node::size);
    uint8_t str_size = (node::size + 7) / 8;
    std::string str_tmp(str_size, 0);
    n.pos <<= sizeof(n.pos) * 8 - node::size;
    for (size_t i = 0; i < str_size; i++)
        str_tmp[i]=(n.pos >> ((sizeof(n.pos) - i - 1) * 8)) & 0xFF;
    bb << str_tmp;
#ifndef NDEBUG
    logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL) << "write ";
    for(const char& c : bb.get_data())
        logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL) << std::bitset<8>(c);
    logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<" : " <<bb.get_size()<<std::endl;
#endif
    return os << bb;
}
sstrtobb& lzw_ns::operator>>(sstrtobb& is, node& n) {
    bytebit bb(node::size);
    n.pos = 0;
    is >> bb;
    uint8_t str_size = bb.get_data().size();
    const std::string& str_tmp = bb.get_data();
    for (size_t i = 0; i < str_size; i++) {
        n.pos<<=8;
        n.pos |= str_tmp[i] & 0xFF;
    }
    if (uint8_t tmp = node::size%8)
        n.pos >>= 8 - tmp;
#ifndef NDEBUG
    logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL) << "read ";
    for(const char& c : bb.get_data())
        logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL) << std::bitset<8>(c);
    logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<" : " <<bb.get_size()<<std::endl;
#endif
    return is;
}

sstrtobb lzw_ns::lzw_0(const std::string& str, std::unordered_map<std::string, size_t> m_slovar) {
    logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL) << "chunk size "<<str.size()<<" buffer size "<<window_buffer_size<<" num_byte "<< size_t(num_byte)<<std::endl;
#ifndef NDEBUG
    size_t slov_szie_start = m_slovar.size();
#endif
    sstrtobb ssbb;
    node n;
    node::size = 1;
    while ((1ULL << node::size) < m_slovar.size())
        node::size++;
    for(size_t i = 0; i < str.size(); ) {
        std::string str_sub = str.substr(i, num_byte);
        std::unordered_map<std::string, size_t>::iterator it = m_slovar.find(str_sub);
#ifndef NDEBUG
        logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<"search start ";
        for(const char& c : str_sub)
            logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<(static_cast<int>(c) & 0xFF)<<' ';
        logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<std::endl;
#endif
        if (it == m_slovar.end()) {
            logger(log_ns::DEV_ONLY) << "ERROR! unknown symbol"<<std::endl;
            throw "EER";
        }
        for (size_t j = num_byte; j < std::min(window_buffer_size * num_byte, str.size() - i); j+=num_byte) {
            str_sub+=str.substr(i + j, num_byte);
            std::unordered_map<std::string, size_t>::iterator it_tmp;
            it_tmp = m_slovar.find(str_sub);
#ifndef NDEBUG
            logger(log_ns::DEV_ONLY | log_ns::HARD_LVL)<<"search ";
            for(const char& c : str_sub)
                logger(log_ns::DEV_ONLY | log_ns::HARD_LVL)<<(static_cast<int>(c) & 0xFF)<<' ';
            logger(log_ns::DEV_ONLY | log_ns::HARD_LVL)<<std::endl;
#endif
            if (it_tmp == m_slovar.end()) {
                logger(log_ns::DEV_ONLY | log_ns::HARD_LVL)<<"not found" << std::endl;
                break;
            }
            it = it_tmp;
        }
        const std::string& str_tmp = it->first;
        i+=str_tmp.size();
#ifndef NDEBUG
        logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<"found ";
        for(const char& c : str_tmp)
            logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<(static_cast<int>(c) & 0xFF)<<' ';
        logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<std::endl;
        logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<"to slovar ";
        for(const char& c : str_tmp + str.substr(i, num_byte))
            logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<(static_cast<int>(c) & 0xFF)<<' ';
        logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<std::endl;
#endif
        n.pos = it->second;
        ssbb << n;
        if (str_tmp.size() / num_byte < window_buffer_size)
            m_slovar[str_tmp + str.substr(i, num_byte)]=m_slovar.size();
        if (m_slovar.size() > (1ULL << node::size))
            node::size++;
    }
    return ssbb;
}

std::string lzw_ns::de_lzw_0(sstrtobb& ssbb, std::vector<std::string> v_slovar) {
    node n;
    std::string last_str;
    std::string str_out;
    node::size = 1;
    sstrtobb ssbb_buffer;
    while ((1ULL << node::size) < v_slovar.size())
        node::size++;
    while (ssbb.get_data().size()) {
        bytebit bb(node::size);
        ssbb >> bb;
        ssbb_buffer << bb;
        ssbb_buffer>>n;
        if (last_str.size() && last_str.size() < window_buffer_size) {
            if (n.pos < v_slovar.size())
                v_slovar.push_back(last_str + v_slovar[n.pos].substr(0, num_byte));
            else
                v_slovar.push_back(last_str + last_str.substr(0, num_byte));
#ifndef NDEBUG
            logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<"to slovar ";
            if (last_str.size() && last_str.size() < window_buffer_size) {
                for(const char& c : v_slovar.back())
                    logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<(static_cast<int>(c) & 0xFF)<<' ';
                logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<std::endl;
            }
#endif
        }
        last_str = v_slovar[n.pos];
        str_out+=last_str;
#ifndef NDEBUG
        if (last_str[0] == 111 && last_str[1] == 116 && last_str[2] == 59)
            logger();
        logger(log_ns::DEV_ONLY) << "write ";
        for (const char& c : last_str)
            logger(log_ns::DEV_ONLY) << (size_t(c) & 0xFF) << ' ';
        logger(log_ns::DEV_ONLY) << std::endl;
#endif
        if (v_slovar.size() >= (1ULL << node::size))
            node::size++;
    }
    return str_out;
}

std::string lzw_ns::lzw_1(const std::string& str) {
    return lzw_ns::lzw_0(str, create_mapslovar_byte()).get_data();
}

#define BUFFER_SIZE window_buffer_size * 5 * num_byte
void lzw(std::istream& stream_in, std::ostream& stream_out) {
    if (window_buffer_size == 0) window_buffer_size = 1 << 26;
    std::string buffer(BUFFER_SIZE, 0);
    sstrtobb ssbb;
    stream_out.put(0);
    while (stream_in) {
        stream_in.read(buffer.data(), BUFFER_SIZE);
        buffer.resize(stream_in.gcount());
        if (buffer.empty()) break;
        sstrtobb new_buffer;
        uint8_t cmin = std::numeric_limits<uint8_t>::max(), cmax = 0;
        // if (num_byte == 1) {
        //     std::for_each(buffer.begin(), buffer.end(), [&cmin, &cmax](const char& c){
        //         cmax = std::max(cmax, static_cast<uint8_t>(c));
        //         cmin = std::min(cmin, static_cast<uint8_t>(c));
        //     });
        //     cmax++;
        //     new_buffer << cmin << cmax;
        // }
        // if (num_byte == 1)
        //     new_buffer << lzw_0(buffer, create_mapslovar_byte(cmin, cmax));
        // else
            new_buffer << lzw_0(buffer, create_mapslovar_byte());
        std::string strsize_ssbb(sizeof(size_t), 0);
        size_t size_ssbb = (new_buffer.get_data().size() - (new_buffer.get_buffer_sdvig_size() ? 1 : 0)) * 8 + new_buffer.get_buffer_sdvig_size();
        std::memcpy(strsize_ssbb.data(), &size_ssbb, sizeof(size_ssbb));
        bytebit bb(strsize_ssbb.size() * 8);
        bb << strsize_ssbb;
        ssbb << bb;
        ssbb<<new_buffer;
        ssbb.try_write(stream_out);
#ifndef NDEBUG
        logger(log_ns::DEV_ONLY) << "write str with size " << size_ssbb<<std::endl;
#endif
    }
    stream_out.write(ssbb.get_data().data(), ssbb.get_data().size());
    stream_out.seekp(0, std::ios::beg);
    stream_out.write(reinterpret_cast<const char*>(&ssbb.get_buffer_sdvig_size()), sizeof(ssbb.get_buffer_sdvig_size()));
}
void de_lzw(std::istream& stream_in, std::ostream& stream_out) {
    if (window_buffer_size == 0) window_buffer_size = 1 << 26;
    std::string buffer(window_buffer_size * 5, 0);
    sstrtobb ssbb;
    uint8_t last_byte = 0;
    stream_in.read(reinterpret_cast<char*>(&last_byte), sizeof(last_byte));
    while (stream_in) {
        stream_in.read(buffer.data(), BUFFER_SIZE);
        buffer.resize(stream_in.gcount());
        ssbb << buffer;
        if (buffer.size() < window_buffer_size * 5) ssbb.set_buffer_sdvig_size(last_byte);
        size_t size_ssbb = 0;
        bytebit ssbbsize_bb(sizeof(size_ssbb) * 8);
        do {
            uint8_t cmin = std::numeric_limits<uint8_t>::max(), cmax = 0;
            ssbb >>ssbbsize_bb;
            std::memcpy(&size_ssbb, ssbbsize_bb.get_data().data(), sizeof(size_ssbb));
            bytebit bb_tmp(size_ssbb
            // + (num_byte == 1 ? 2 * 8 * sizeof(cmin) : 0)
            );
            ssbb >> bb_tmp;
            sstrtobb ssbb_tmp;
            ssbb_tmp << bb_tmp;
            // if (num_byte == 1)
            //     ssbb_tmp >> cmin >> cmax;
            std::string str;
            // if (num_byte == 1)
            //     str = de_lzw_0(ssbb_tmp, create_slovar_byte(cmin, cmax));
            // else
                str = de_lzw_0(ssbb_tmp, create_slovar_byte());
            stream_out.write(str.data(), str.size());
        } while(size_ssbb <= ((ssbb.get_data().size() - 1) * 8 + ssbb.get_buffer_sdvig_size()));
    }
}
#undef BUFFER_SIZE
