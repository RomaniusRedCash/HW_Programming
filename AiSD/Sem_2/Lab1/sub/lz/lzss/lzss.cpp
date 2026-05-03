#include "lzss.h"

using namespace lzss_ns;

sstrtobb& lzss_ns::operator<<(sstrtobb& os, const node& n) {
    if (n.pos == 3) {
        logger();
    }
    bytebit bb(0);
    if (n.len < (node::size_len + 7) / 8 + 1) {
        logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL) << "write 0 "<<n.data << std::endl;
        bb.add_null();
        os << bb;
        os << n.data;
    }
    else {
        logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL) << "write 1 "<< size_t(n.pos)<< ' ' << size_t(n.len) << std::endl;
        bb.add_one();
        os << bb;
        bytebit bb_tmp(node::size_len);
        size_t tmp = n.pos;
        uint8_t str_size = (node::size_len + 7) / 8;
        std::string str_tmp(str_size, 0);
        tmp <<= sizeof(tmp) * 8 - node::size_len;
        for (size_t i = 0; i < str_size; i++)
            str_tmp[i]=(tmp >> ((sizeof(tmp) - i - 1) * 8)) & 0xFF;
        bb_tmp << str_tmp;
        os << bb_tmp;
        bb_tmp.clear();
        bb_tmp.set_size(node::size_len);
        tmp = n.len;
        tmp <<= sizeof(tmp) * 8 - node::size_len;
        for (size_t i = 0; i < str_size; i++)
            str_tmp[i]=(tmp >> ((sizeof(tmp) - i - 1) * 8)) & 0xFF;
        bb_tmp << str_tmp;
        os << bb_tmp;
    }
    return os;
}

sstrtobb& lzss_ns::operator>>(sstrtobb& is, node& n) {
    bytebit bb(1);
    is >> bb;
    if (bb.get_data()[0] == 0) {
        n.data.resize(num_byte);
        is >> n.data;
        logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL) << "read 0 "<< n.data << std::endl;
    }
    else {
        // is >> n.pos >> n.len;
        n.pos = 0;
        n.len = 0;
        bytebit bb_tmp(node::size_len);
        is >> bb_tmp;
        uint8_t str_size = bb_tmp.get_data().size();
        const std::string* str_tmp = &bb_tmp.get_data();
        for (size_t i = 0; i < str_size; i++) {
            n.pos<<=8;
            n.pos |= (*str_tmp)[i] & 0xFF;
        }
        if (uint8_t tmp = node::size_len%8)
            n.pos >>= 8 - tmp;
        bb_tmp.clear();
        bb_tmp.set_size(node::size_len);
        is >> bb_tmp;
        str_tmp = &bb_tmp.get_data();
        for (size_t i = 0; i < str_size; i++) {
            n.len<<=8;
            n.len |= (*str_tmp)[i] & 0xFF;
        }
        if (uint8_t tmp = node::size_len%8)
            n.len >>= 8 - tmp;

        logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL) << "read 1 "<< size_t(n.pos)<< ' ' << size_t(n.len) << std::endl;
    }
    return is;
}

#define BUFFER_SIZE 10 * num_byte * buffer_size
void lzss_ns::lzss_1(std::istream& stream_in, std::ostream& stream_out) {
    node::size_len = 0;
    while ((1ULL << node::size_len) < buffer_size_lz)
        node::size_len++;
// last byte
    logger()<<"buffer size is "<<buffer_size_lz<<std::endl;
    stream_out.put(0);
    // if (buffer_size >= 256) throw "ERR";
    size_t buffer_size = buffer_size_lz * num_byte;
    sstrtobb bbs_out;
#ifndef NDEBUG
    sstrtobb bbs_dbg;
#endif
    std::string buffer(BUFFER_SIZE, 0);
    size_t read_bites = 0;
    while(stream_in) {
        stream_in.read(buffer.data(), BUFFER_SIZE);
        read_bites = stream_in.gcount();
        for (size_t i = 0; i < read_bites;) {
            node n1;
            size_t start = 0;
            if (i > buffer_size)
                start = i - buffer_size;
            std::string_view str_vi(buffer.data() + start, std::min(buffer_size, i));
            logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<"str_vi is "<<str_vi<<std::endl;
            for (size_t j = i + num_byte; j != std::min(i + buffer_size, buffer.size()) && j - i <= str_vi.size(); j+=num_byte){
                logger(log_ns::DEV_ONLY | log_ns::HARD_LVL) << "search "<<std::string_view(buffer.data() + i, j - i)<<std::endl;
                size_t tmp_pos = str_vi.find(std::string_view(buffer.data() + i, j - i), (n1.pos ? str_vi.size() - n1.pos : 0));
                if (tmp_pos == str_vi.npos) {
                    logger(log_ns::DEV_ONLY | log_ns::HARD_LVL) <<"not found. last is "<<n1.pos<<' '<<n1.len<<std::endl;
                    break;
                }
                logger(log_ns::DEV_ONLY | log_ns::HARD_LVL) << "tmp_pos is "<<tmp_pos<<std::endl;
                n1.pos = str_vi.size() - tmp_pos;
                n1.len++;
            }
            n1.pos /= num_byte;
            n1.data = buffer.substr(i, num_byte);
            i+=((n1.len < (node::size_len + 7) / 8 + 1) ? 1 : n1.len)*num_byte;
#ifndef NDEBUG
            if (str_vi.size() > buffer_size) {
                logger(log_ns::DEV_ONLY)<<"ERROR! len str_vi buffer"<<std::endl;
                throw "ERR";
            }
            if(n1.len) {
                logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<"found "<<std::string_view(str_vi.data()+str_vi.size()-n1.pos,n1.len)<<std::endl;
            }
            logger(log_ns::DEV_ONLY| log_ns::NORMAL_LVL)<<"pos "<<size_t(n1.pos)<<" len "<<size_t(n1.len)<<' '<<n1.data<<std::endl;
#endif
            bbs_out<<n1;
#ifndef NDEBUG
            bbs_dbg<<n1;
            node n_dbg;
            bbs_dbg>>n_dbg;
            if (n_dbg.data.empty()) {
                if (n1.pos != n_dbg.pos) {
                    logger(log_ns::DEV_ONLY)<<"ERROR! pos "<<n1.pos<<" != " <<n_dbg.pos<<std::endl;
                    throw "ERR";
                } else if (n1.len != n_dbg.len) {
                    logger(log_ns::DEV_ONLY)<<"ERROR! len "<<n1.len<<" != " <<n_dbg.len<<std::endl;
                    throw "ERR";
                }
            } else if (n_dbg.data != n1.data) {
                logger(log_ns::DEV_ONLY)<<"ERROR! data "<<n1.data<<" != " <<n_dbg.data<<std::endl;
                throw "ERR";
            }
            if (n1.len > buffer_size_lz) {
                logger(log_ns::DEV_ONLY)<<"ERROR! buffer len "<<n1.pos<<" > " <<buffer_size_lz<<std::endl;
                throw "ERR";
            } else if (n1.pos > buffer_size_lz) {
                logger(log_ns::DEV_ONLY)<<"ERROR! buffer pos "<<n1.pos<<" > " <<buffer_size_lz<<std::endl;
                throw "ERR";
            }
#endif
            bbs_out.try_write(stream_out);
        }
        if (read_bites < BUFFER_SIZE) break;
    }
    stream_out.write(bbs_out.get_data().data(), bbs_out.get_data().size());
    stream_out.clear();
    stream_out.seekp(0, std::ios::beg);
    stream_out.write(reinterpret_cast<const char*>(&bbs_out.real_last()), sizeof(bbs_out.real_last()));
}

void lzss_ns::de_lzss_1(std::istream& stream_in, std::ostream& stream_out) {
    node::size_len = 0;
    while ((1ULL << node::size_len) < buffer_size_lz)
        node::size_len++;
    uint8_t last_byte_size = 0;
    stream_in.read(reinterpret_cast<char*>(&last_byte_size), sizeof(last_byte_size));
    // if (buffer_size >= 256) throw "ERR";
    // buffer_size *= num_byte;
    size_t buffer_size = buffer_size_lz * num_byte;
    logger(log_ns::DEV_ONLY) <<"last byte len: "<<size_t(last_byte_size) << std::endl;
    std::string buffer(BUFFER_SIZE, 0);
    sstrtobb ssbb;
    std::string ss_buffer;
    while(stream_in) {
        stream_in.read(buffer.data(), BUFFER_SIZE);
        size_t read_bites = stream_in.gcount();
        char c;
        if (stream_in.get(c))
            stream_in.seekg(-1, std::ios::cur);
        else buffer.resize(read_bites);
#ifndef NDEBUG
        logger(log_ns::DEV_ONLY | log_ns::HARD_LVL)<<"read ";
        logger(log_ns::DEV_ONLY | log_ns::HARD_LVL).write(buffer.data(), read_bites);
        logger(log_ns::DEV_ONLY | log_ns::HARD_LVL)<<" size "<<read_bites<<std::endl;
        logger(log_ns::DEV_ONLY)<<"new buffer is ";
        for (const char& c : buffer)
            logger(log_ns::DEV_ONLY)<<std::bitset<8>(c) << ' ';
        logger(log_ns::DEV_ONLY)<<std::endl;
#endif
        if (read_bites < BUFFER_SIZE) {
            sstrtobb ssbb_tmp(buffer);
            ssbb_tmp.set_buffer_sdvig_size(last_byte_size);
            ssbb << ssbb_tmp;
        } else ssbb<<buffer;
        while (ssbb.get_data().size() > (node::size_len + 7) / 8 * 2 || (stream_in.eof() && ssbb.get_data().size())) {
            node n;
            if (!try_read_node(ssbb, n)){
                logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<"next buffer"<<std::endl;
                break;
            }
            if (!n.len){
                logger(log_ns::DEV_ONLY | log_ns::HARD_LVL)<<"buffer is "<<ss_buffer<<" write 0 "<<n.data<<std::endl;
                ss_buffer+=n.data;
                logger(log_ns::DEV_ONLY | log_ns::HARD_LVL)<<"n.data.size() "<<n.data.size()<<std::endl;
                stream_out.write(n.data.data(), n.data.size());
            } else {
                logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<" write 1 "<< std::string_view(ss_buffer.data() + ss_buffer.size() - n.pos * num_byte, n.len * num_byte)<<std::endl;
                std::string_view str_vw(ss_buffer.data() + ss_buffer.size() - n.pos * num_byte, n.len * num_byte);
                stream_out.write(str_vw.data(), str_vw.size());
                ss_buffer += str_vw;
            }
            if (ss_buffer.size() > buffer_size) {
                logger(log_ns::DEV_ONLY | log_ns::HARD_LVL)<<"buffer is " <<ss_buffer << " size " << ss_buffer.size() << " > " << buffer_size<<std::endl;
                ss_buffer.erase(ss_buffer.begin(), ss_buffer.begin() + ss_buffer.size() - buffer_size);
                if (ss_buffer.size() != buffer_size) {
                    logger(log_ns::DEV_ONLY)<<"ERROR! buffer len "<<ss_buffer.size() <<" != "<<buffer_size<<std::endl;
                    throw "ERR";
                }
            }
        }
    }
}
#undef BUFFER_SIZE

bool lzss_ns::try_read_node(sstrtobb& ssbb_in, node& n) {
    if (ssbb_in.get_data().front() & 0x80 &&
        ssbb_in.get_data().size() >= 3
    ) {
        ssbb_in >> n;
        return true;
    } else if (!(ssbb_in.get_data().front() & 0x80) &&
        ssbb_in.get_data().size() >= 1 + n.data.size()) {
        ssbb_in >> n;
        return true;
    }
    return false;
}


void lzss(std::istream& stream_in, std::ostream& stream_out) {
    lzss_1(stream_in, stream_out);
}
void de_lzss(std::istream& stream_in, std::ostream& stream_out) {
    de_lzss_1(stream_in, stream_out);
}
