#include "ha.h"
#include "ha_sub.h"
#include <cstdint>
#include <cstdlib>
#include <string>

using namespace ha_ns;

//////////////////////////////////// fun ////////////////////////////////////
/*
 * last byte size
 * byte of model size
 * model size
 * byte of node size
 * node size
*/

#define BUFFER_SIZE 1024 * num_byte
void ha_ns::ha_1(std::istream& stream_in, std::ostream& stream_out, const uint8_t& num_byte, const calculator& calc) {
// last byte
    stream_out.put(0);

// model size
    write_shift_size(stream_out, calc.get_size());
    write_shift_size(stream_out, calc);

    sstrtobb bbs_out;
    std::string buffer(BUFFER_SIZE, 0);
    size_t read_bites = 0;

    while(stream_in) {
        stream_in.read(buffer.data(), BUFFER_SIZE);
        read_bites = stream_in.gcount();
        for (size_t i = 0; i < read_bites / num_byte; i++) {
            bbs_out<<calc[buffer.substr(i * num_byte, num_byte)];
            bbs_out.try_write(stream_out);
        }
        if (read_bites < BUFFER_SIZE) break;
    }
    if (size_t ost = read_bites % num_byte)
        bbs_out<<calc[buffer.substr(read_bites - ost, ost)];
    stream_out << bbs_out.get_data();
    stream_out.clear();
    stream_out.seekp(0, std::ios::beg);
    stream_out << bbs_out.real_last();
}

void ha(std::istream& stream_in, std::ostream& stream_out, const uint8_t& num_byte) {
    std::map<std::string, size_t> mapa;
    std::string buffer(BUFFER_SIZE, 0);
    size_t read_bites = 0;
    while(stream_in) {
        stream_in.read(buffer.data(), BUFFER_SIZE);
        read_bites = stream_in.gcount();
        logger(log_ns::DEV_ONLY | log_ns::HARD_LVL)<<"read ";
        logger(log_ns::DEV_ONLY | log_ns::HARD_LVL).write(buffer.data(), read_bites);
        logger(log_ns::DEV_ONLY | log_ns::HARD_LVL)<<" size "<<read_bites<<std::endl;
        for (size_t i = 0; i < read_bites / num_byte; i++) {
            mapa[buffer.substr(i * num_byte, num_byte)]++;
        }
        if (read_bites < BUFFER_SIZE) break;
    }
    if (size_t ost = read_bites % num_byte)
        mapa[buffer.substr(read_bites - ost, ost)]++;
    std::vector<node*> v_nodes;
    logger()<<"nabor: "<<std::endl;
    for (const std::pair<std::string, size_t> i : mapa) {
        node* nn = new node(i.first, i.second);
        v_nodes.push_back(nn);
        logger()<<i.first<<" : " <<i.second<<std::endl;
    }
    calculator calc(v_nodes);
    stream_in.clear();
    stream_in.seekg(0, std::ios::beg);
    ha_1(stream_in,stream_out,num_byte,calc);
}

void de_ha(std::istream& stream_in, std::ostream& stream_out, const uint8_t& num_byte) {
    uint8_t last_byte_size = 0;
    size_t model_size = 0;
    stream_in >> last_byte_size;
    read_shift_size(stream_in, model_size);
    logger(log_ns::DEV_ONLY) <<"last byte len: "<<size_t(last_byte_size) << " model size: "<<model_size<<std::endl;
    std::map<size_t, std::vector<std::string>> mapa;
    uint8_t node_size;
    stream_in >> node_size;
    read_shift_size(stream_in, mapa, model_size, num_byte, node_size);
    size_t max_size_sim = std::prev(mapa.end())->first;
    calculator calc(mapa);

    std::string buffer(BUFFER_SIZE, 0);
    sstrtobb ssbb;
    while(!stream_in.eof()) {
        stream_in.read(buffer.data(), BUFFER_SIZE);
        size_t read_bites = stream_in.gcount();

        char c;
        buffer.resize(read_bites);

        logger(log_ns::DEV_ONLY | log_ns::HARD_LVL)<<"read ";
        logger(log_ns::DEV_ONLY | log_ns::HARD_LVL).write(buffer.data(), read_bites);
        logger(log_ns::DEV_ONLY | log_ns::HARD_LVL)<<" size "<<read_bites<<std::endl;

#ifndef NDEBUG
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
        while (ssbb.get_data().size()) { // NOTE: Переделать. Так как часто делает сдвиги.
            ha_code hc;
            if (!hc.read_hacode(ssbb, calc, max_size_sim)) {
                logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<"next buffer"<<std::endl; // это вообще я дебилизм написал
                break;
            }
            stream_out<<calc[hc];
        }
    }
}
#undef BUFFER_SIZE































