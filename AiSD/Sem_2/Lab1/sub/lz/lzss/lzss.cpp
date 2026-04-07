#include "lzss.h"

using namespace lzss_ns;

sstrtobb& lzss_ns::operator<<(sstrtobb& os, const node& n) {
    bytebit bb(0);
    if (n.len <= 1){
        bb.add_null();
        os << bb << n.data;
    }
    else {
        bb.add_one();
        os << bb << n.pos << n.len;
    }
    return os;
}

sstrtobb& lzss_ns::operator>>(sstrtobb& is, node& n) {
    bytebit bb(1), bb2(n.data.size());
    is >> bb;
    if (bb.get_data()[0] == 0)
        is >> bb2;
    else
        is >> n.pos >> n.len;
    n.data = bb.get_data();
    return is;
}

// std::string lzss_0(const std::string& str, size_t buffer_size, const uint8_t& num_byte) {
//     buffer_size *= num_byte;
//     sstrtobb ss;
//     if (buffer_size > 256) throw "ERR";
//     for (size_t i = 0; i < str.size(); i+=num_byte) {
//         node n1;
//         size_t start = 0;
//         if (i >= buffer_size)
//             start = i - buffer_size;
//         std::string_view str_vi(str.data() + start, std::min(buffer_size, i));
//         logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<"str_vi is "<<str_vi<<std::endl;
//         for (size_t j = i + num_byte; j != std::min(i + buffer_size, str.size()) && j - i + num_byte <= str_vi.size(); j+=num_byte){
//             logger(log_ns::DEV_ONLY | log_ns::HARD_LVL) << "search "<<std::string_view(str.data() + i, j - i)<<std::endl;
//             size_t tmp_pos = str_vi.find(std::string_view(str.data() + i, j - i), (n1.pos == 0 ? 0 :str_vi.size() - n1.pos));
//             if (tmp_pos == str_vi.npos) {
//                 logger(log_ns::DEV_ONLY | log_ns::HARD_LVL) <<"not found. last is "<<n1.pos<<' '<<n1.len<<std::endl;
//                 break;
//             }
//             logger(log_ns::DEV_ONLY | log_ns::HARD_LVL) << "tmp_pos is "<<tmp_pos<<std::endl;
//             n1.pos = str_vi.size() - tmp_pos;
//             n1.len++;
//         }
//         n1.pos /= num_byte;
//         n1.data = str[i];
//         // str.substr(i + n1.len,num_byte);
//         i+=n1.len*num_byte;
//         #ifdef DEBUG
//         if(n1.len) {
//             logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<"found "<<std::string_view(str.data()+n1.pos,n1.len)<<std::endl;
//         }
//         logger(log_ns::DEV_ONLY| log_ns::NORMAL_LVL)<<"pos "<<n1.pos<<" len "<<n1.len<<' '<<n1.data<<std::endl;
//         #endif
//         ss<<n1;
//     }
//     return ss;
// }

#define BUFFER_SIZE 1024 * num_byte
void lzss_ns::lzss_0(std::istream& stream_in, std::ostream& stream_out, const uint8_t& num_byte, size_t buffer_size) {
    // last byte
    stream_out.put(0);

    if (buffer_size > 256) throw "ERR";
    buffer_size *= num_byte;

    sstrtobb bbs_out;
    std::string buffer(BUFFER_SIZE, 0);
    size_t read_bites = 0;

    while(stream_in) {
        stream_in.read(buffer.data(), BUFFER_SIZE);
        read_bites = stream_in.gcount();
        for (size_t i = 0; i < read_bites; i+=num_byte) {
            node n1;
            size_t start = 0;
            if (i >= buffer_size)
                start = i - buffer_size;
            std::string_view str_vi(buffer.data() + start, std::min(buffer_size, i));
            logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<"str_vi is "<<str_vi<<std::endl;
            for (size_t j = i + num_byte; j != std::min(i + buffer_size, buffer.size()) && j - i + num_byte <= str_vi.size(); j+=num_byte){
                logger(log_ns::DEV_ONLY | log_ns::HARD_LVL) << "search "<<std::string_view(buffer.data() + i, j - i)<<std::endl;
                size_t tmp_pos = str_vi.find(std::string_view(buffer.data() + i, j - i), (n1.pos == 0 ? 0 :str_vi.size() - n1.pos));
                if (tmp_pos == str_vi.npos) {
                    logger(log_ns::DEV_ONLY | log_ns::HARD_LVL) <<"not found. last is "<<n1.pos<<' '<<n1.len<<std::endl;
                    break;
                }
                logger(log_ns::DEV_ONLY | log_ns::HARD_LVL) << "tmp_pos is "<<tmp_pos<<std::endl;
                n1.pos = str_vi.size() - tmp_pos;
                n1.len++;
            }
            n1.pos /= num_byte;
            n1.data = buffer[i];
            i+=n1.len*num_byte;
#ifdef DEBUG
            if(n1.len) {
                logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<"found "<<std::string_view(str.data()+n1.pos,n1.len)<<std::endl;
            }
            logger(log_ns::DEV_ONLY| log_ns::NORMAL_LVL)<<"pos "<<n1.pos<<" len "<<n1.len<<' '<<n1.data<<std::endl;
#endif
            bbs_out<<n1;
            bbs_out.try_write(stream_out);
        }
        if (read_bites < BUFFER_SIZE) break;
    }
    stream_out << bbs_out.get_data();
    stream_out.clear();
    stream_out.seekp(0, std::ios::beg);
    stream_out << bbs_out.real_last();
}

void lzss_ns::de_lzss_0(std::istream& stream_in, std::ostream& stream_out, const uint8_t& num_byte, size_t buffer_size) {
    uint8_t last_byte_size = 0;
    stream_in >> last_byte_size;
    logger(log_ns::DEV_ONLY) <<"last byte len: "<<size_t(last_byte_size);
    uint8_t node_size;
    stream_in >> node_size;

    std::string buffer(BUFFER_SIZE, 0);
    sstrtobb ssbb;
    while(!stream_in.eof()) {
        stream_in.read(buffer.data(), BUFFER_SIZE);
        size_t read_bites = stream_in.gcount();

        char c;
        if (stream_in.get(c))
            stream_in.seekg(-1, std::ios::cur);
        else buffer.resize(read_bites);

        logger(log_ns::DEV_ONLY | log_ns::HARD_LVL)<<"read ";
        logger(log_ns::DEV_ONLY | log_ns::HARD_LVL).write(buffer.data(), read_bites);
        logger(log_ns::DEV_ONLY | log_ns::HARD_LVL)<<" size "<<read_bites<<std::endl;

#ifdef DEBUG
        logger(log_ns::DEV_ONLY)<<"new buffer is ";
        for (const char& c : buffer)
            logger(log_ns::DEV_ONLY)<<std::bitset<8>(c);
        logger(log_ns::DEV_ONLY)<<std::endl;
#endif


        if (stream_in.eof()){
            sstrtobb ssbb_tmp(buffer);
            ssbb_tmp.set_buffer_sdvig_size(last_byte_size);
            ssbb << ssbb_tmp;
        } else ssbb<<buffer;
        // ssbb.set_buffer_sdvig_size((ssbb.get_buffer_sdvig_size() + last_byte_size - 8) % 8);
    }
}

#undef BUFFER_SIZE


void lzss(std::istream& stream_in, std::ostream& stream_out, const uint8_t& num_byte);
void de_lzss(std::istream& stream_in, std::ostream& stream_out, const uint8_t& num_byte);
