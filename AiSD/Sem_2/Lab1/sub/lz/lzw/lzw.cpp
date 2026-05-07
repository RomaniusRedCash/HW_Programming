#include "lzw.h"

using namespace lzw_ns;

init_data lzw_ns::read_param() {
    init_data out;
    if (sub_commands.front().empty()) return out;
    std::string segment;
    std::stringstream ss(sub_commands.front());
    // std::getline(ss, segment, ',');
    // out.window = lz::get_window(segment, 1<<26);
    std::getline(ss, segment, ',');
    if (segment.size() > 1)
        logger()<<"unknown style"<<std::endl;
    else switch (segment.front()) {
            case 'd':
                out.style = defaultlzw;
                break;
            case 'f':
                out.style = full;
                break;
            default:
                logger()<<"unknown style"<<std::endl;
                break;
        }
    std::getline(ss, segment, ',');
    try {
        out.size_slovar = std::stoi(segment);
    } catch (...) {
        logger() << "unknown parametrs: "<<sub_commands.front()<<std::endl;
    }
    if (out.size_slovar == 0)
        out.size_slovar = std::numeric_limits<size_t>::max();
    return out;
}

std::vector<std::string> lzw_ns::create_slovar_byte() {
    std::vector<std::string> v_slovar(1 << (num_byte * 8), std::string(num_byte, 0));
    for (size_t i = 0; i < 1 << (num_byte * 8); i++)
        std::memcpy(v_slovar[i].data(), &i, num_byte);
    return v_slovar;
}

std::vector<std::string> lzw_ns::create_slovar_byte(std::istream& is) {
    size_t size = 0;
    std::string str_tmp(num_byte, 0);
    is.read(str_tmp.data(), num_byte);
    std::memcpy(&size, str_tmp.data(), num_byte);
    std::vector<std::string> v_slovar(size, std::string(num_byte, 0));
    for (size_t i = 0; i < size; i++) {
        is.read(str_tmp.data(), num_byte);
        v_slovar[i]=str_tmp;
    }
    return v_slovar;
}

std::unordered_map<std::string, size_t> lzw_ns::create_mapslovar_byte() {
    std::unordered_map<std::string, size_t> map;
    for (size_t i = 0; i < 1 << (num_byte * 8); i++) {
        std::string str(num_byte, 0);
        std::memcpy(str.data(), &i, num_byte);
        map[str] = i;
    }
    return map;
}

std::unordered_map<std::string, size_t> lzw_ns::create_mapslovar_byte(std::istream& is, std::ostream& os) {
    size_t start_pos = is.tellg();
    std::unordered_map<std::string, size_t> mapa;
    std::vector<std::string> pool;
    std::string str(num_byte, 0);
    while (is.read(str.data(), num_byte))
        if (mapa.find(str) == mapa.end()) {
            mapa[str]=mapa.size();
            pool.push_back(str);
        }
    is.clear();
    is.seekg(start_pos);

    std::string str_tmp(num_byte, 0);
    size_t tmp_size = pool.size();
    std::memcpy(str_tmp.data(), &tmp_size, num_byte);
    os<<str_tmp;
    logger(log_ns::DEV_ONLY) << "slovar gen:"<<std::endl;
    for (size_t i = 0; i < tmp_size; i++) {
        for (const uint8_t& c : pool[i])
            logger(log_ns::DEV_ONLY)<<size_t(c)<<' ';
        logger(log_ns::DEV_ONLY)<<" : " << i<<std::endl;
        os.write(pool[i].data(), num_byte);
    }
    return {mapa.begin(), mapa.end()};
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

sstrtobb lzw_ns::lzw_0(const std::string& str, std::unordered_map<std::string, size_t>& m_slovar, const init_data& idata) {
    logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL) << "chunk size "<<str.size()<<" num_byte "<< size_t(num_byte)<<std::endl;
#ifndef NDEBUG
    size_t slov_szie_start = m_slovar.size();
#endif
    sstrtobb ssbb;
    node n;
    node::size = 1;
    while ((1ULL << node::size) < m_slovar.size())
        node::size++;
    for(size_t i = 0; i < str.size() / num_byte * num_byte; ) {
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
        for (size_t j = num_byte; j < str.size() - i; j+=num_byte) {
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
        if (m_slovar.size() < idata.size_slovar) {
            logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<"to slovar ";
            for(const char& c : str_tmp + str.substr(i, num_byte))
                logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<(static_cast<int>(c) & 0xFF)<<' ';
            logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<< ": " << m_slovar.size()<<std::endl;
        }
#endif
        n.pos = it->second;
        ssbb << n;
        if (m_slovar.size() < idata.size_slovar)
            m_slovar[str_tmp + str.substr(i, num_byte)]=m_slovar.size();
        if (m_slovar.size() > (1ULL << node::size)) {
            node::size++;
            logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL) << "node size up "<<node::size<<std::endl;
        }
    }
    // if (size_t tmp = str.size()%num_byte) {
        // ssbb<<str.substr(str.size() - tmp, tmp);
    // }
    return ssbb;
}

std::string lzw_ns::de_lzw_0(sstrtobb& ssbb, std::vector<std::string>& v_slovar, const init_data& idata) {
    node n;
    std::string last_str;
    std::string str_out;
    node::size = 1;
    sstrtobb ssbb_buffer;

    while ((1ULL << node::size) < v_slovar.size())
        node::size++;
    while (((ssbb.get_data().size() - (ssbb.get_buffer_sdvig_size() ? 1 : 0)) * 8 + ssbb.get_buffer_sdvig_size()) >= node::size ||
        ((ssbb_buffer.get_data().size() - (ssbb_buffer.get_buffer_sdvig_size() ? 1 : 0)) * 8 + ssbb_buffer.get_buffer_sdvig_size()) >= node::size
    ) {
        if (((ssbb_buffer.get_data().size() - (ssbb_buffer.get_buffer_sdvig_size() ? 1 : 0)) * 8 + ssbb_buffer.get_buffer_sdvig_size()) < node::size) {
#ifndef NDEBUG
            logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<"ost ssbb_buffer "<<(ssbb_buffer.get_data().size() - (ssbb_buffer.get_buffer_sdvig_size() ? 1 : 0)) * 8 + ssbb_buffer.get_buffer_sdvig_size();
            logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<" ost ssbb "<<(ssbb.get_data().size() - (ssbb.get_buffer_sdvig_size() ? 1 : 0)) * 8 + ssbb.get_buffer_sdvig_size();
#endif
            bytebit bb(std::min(node::size * 800, (ssbb.get_data().size() - (ssbb.get_buffer_sdvig_size() ? 1 : 0)) * 8 + ssbb.get_buffer_sdvig_size()));
            ssbb >> bb;
            ssbb_buffer << bb;
#ifndef NDEBUG
            logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<"teper ssbb_buffer "<<(ssbb_buffer.get_data().size() - (ssbb_buffer.get_buffer_sdvig_size() ? 1 : 0)) * 8 + ssbb_buffer.get_buffer_sdvig_size();
            logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<" teper ssbb "<<(ssbb.get_data().size() - (ssbb.get_buffer_sdvig_size() ? 1 : 0)) * 8 + ssbb.get_buffer_sdvig_size();
            logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<std::endl<<" bb " << bb.get_size()<<std::endl;
#endif
        }
        ssbb_buffer>>n;
        if (last_str.size() && v_slovar.size() < idata.size_slovar) {
            if (n.pos < v_slovar.size())
                v_slovar.push_back(last_str + v_slovar[n.pos].substr(0, num_byte));
            else
                v_slovar.push_back(last_str + last_str.substr(0, num_byte));
#ifndef NDEBUG
            logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<"to slovar ";
            for(const char& c : v_slovar.back())
                logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<(static_cast<int>(c) & 0xFF)<<' ';
            logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<< ": " << v_slovar.size() - 1 <<std::endl;
#endif
        }
        last_str = v_slovar[n.pos];
        str_out+=last_str;
#ifndef NDEBUG
        logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL) << "write ";
        for (const char& c : last_str)
            logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL) << (size_t(c) & 0xFF) << ' ';
        logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL) << std::endl;
#endif
        if (v_slovar.size() >= (1ULL << node::size)) {
            node::size++;
            logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL) << "node size up "<<node::size<<std::endl;
        }
    }
    logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL) << "buffer end"<<std::endl;
    return str_out;
}

// std::string lzw_ns::lzw_1(const std::string& str, const init_data& idata) {
//     return lzw_ns::lzw_0(str, create_mapslovar_byte(), idata).get_data();
// }

#define BUFFER_SIZE (1<<16) * sizeof(size_t) * num_byte
void lzw(std::istream& stream_in, std::ostream& stream_out) {
    init_data param = read_param();
    std::unordered_map<std::string, size_t> slovar;
    if (param.style == lzw_ns::full)
        slovar = create_mapslovar_byte(stream_in, stream_out);
    else
        slovar = create_mapslovar_byte();
    if (param.size_slovar < slovar.size()) {
        logger()<<"slovar is very low. must be more then "<<slovar.size()<<std::endl;
        throw "ERR";
    }
    std::string buffer(BUFFER_SIZE, 0);
    sstrtobb ssbb;
    while (stream_in) {
        stream_in.read(buffer.data(), BUFFER_SIZE);
        buffer.resize(stream_in.gcount());
        if (buffer.empty()) break;
        sstrtobb new_buffer;
        new_buffer << lzw_0(buffer, slovar, param);
        std::string strsize_ssbb(sizeof(size_t), 0);
        size_t size_ssbb = (new_buffer.get_data().size() - (new_buffer.get_buffer_sdvig_size() ? 1 : 0)) * 8 + new_buffer.get_buffer_sdvig_size();
        std::memcpy(strsize_ssbb.data(), &size_ssbb, sizeof(size_ssbb));
        bytebit bb(strsize_ssbb.size() * 8);
        bb << strsize_ssbb;
        ssbb << bb;
        ssbb<<new_buffer;
#ifndef NDEBUG
        logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL) << "write str with size " << size_ssbb<<std::endl;
#endif
        ssbb.try_write(stream_out);
    }
    stream_out.write(ssbb.get_data().data(), ssbb.get_data().size());
    logger(log_ns::DEV_ONLY)<<"slovar size: "<<slovar.size()<<std::endl;
}
void de_lzw(std::istream& stream_in, std::ostream& stream_out) {
    init_data param = read_param();
    std::vector<std::string> slovar;
    if (param.style == lzw_ns::full) {
        slovar = create_slovar_byte(stream_in);
        logger(log_ns::DEV_ONLY) << "slovar read:"<<std::endl;
        for (size_t i = 0; i < slovar.size(); i++) {
            for (const uint8_t& c : slovar[i])
                logger(log_ns::DEV_ONLY)<<size_t(c)<<' ';
            logger(log_ns::DEV_ONLY)<<" : " << i <<std::endl;
        }
    }
    else
        slovar = create_slovar_byte();
    if (param.size_slovar < slovar.size()) {
        logger()<<"slovar is very low. must be more then "<<slovar.size()<<std::endl;
        throw "ERR";
    }
    std::string buffer(BUFFER_SIZE, 0);
    sstrtobb ssbb;
    while (stream_in) {
        stream_in.read(buffer.data(), BUFFER_SIZE);
        buffer.resize(stream_in.gcount());
        if (buffer.empty()) break;
        ssbb << buffer;
        size_t size_ssbb = 0;
        do {
            bytebit ssbbsize_bb(sizeof(size_ssbb) * 8);
            ssbb >>ssbbsize_bb;
            std::memcpy(&size_ssbb, ssbbsize_bb.get_data().data(), sizeof(size_ssbb));
            bytebit bb_tmp(size_ssbb);
            if (size_ssbb > ((ssbb.get_data().size() - (ssbb.get_buffer_sdvig_size() ? 1 : 0)) * 8 + ssbb.get_buffer_sdvig_size())) {
                if (stream_in.eof()) throw "ERR";
                stream_in.read(buffer.data(), BUFFER_SIZE);
                buffer.resize(stream_in.gcount());
                ssbb << buffer;
            }
            ssbb >> bb_tmp;
            sstrtobb ssbb_tmp;
            ssbb_tmp << bb_tmp;
            std::string str;
            str = de_lzw_0(ssbb_tmp, slovar, param);
            stream_out.write(str.data(), str.size());
        } while(buffer.size() >= BUFFER_SIZE && ((ssbb.get_data().size() - (ssbb.get_buffer_sdvig_size() ? 1 : 0)) * 8 + ssbb.get_buffer_sdvig_size()) >= sizeof(size_ssbb)*8);
    }
    logger(log_ns::DEV_ONLY)<<"slovar size: "<<slovar.size()<<std::endl;
}
#undef BUFFER_SIZE
