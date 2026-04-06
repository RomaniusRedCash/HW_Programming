#include "ha_sub.h"

void ha_ns::write_shift_size(std::ostream& stream_out, const size_t& size) {
    uint8_t size_shift = 0;
    for (size_shift = 0; size >> (size_shift * 8); size_shift++);
    stream_out << size_shift;
    // size_t value = size << (sizeof(size) - size_shift * 8);
    logger() <<"write size "<< size <<" with shift "<<size_t(size_shift)<< std::endl;
    stream_out.write(reinterpret_cast<const char*>(&size), size_shift);
}

void ha_ns::write_shift_size(std::ostream& stream_out, const calculator& calc) {
    size_t max_size = 0;
    uint8_t size_shift = 0;
    for (const std::pair<std::string, ha_code>& i :calc)
        max_size = std::max(i.second.get_size(), max_size);
    for (size_shift = 0; max_size >> (size_shift * 8); size_shift++);
    logger() << "max node size: " <<max_size<<" shift: "<<int(size_shift)<<std::endl;
    stream_out << size_shift;
    for (const std::string& i :calc.get_ord()) {
        stream_out<<i;
        // size_t value = i.second.get_size() << (sizeof(i.second.get_size()) - size_shift) * 8;
        stream_out.write(reinterpret_cast<const char*>(&calc.get_mapa().at(i).get_size()), size_shift);

#ifdef DEBUG
        logger(log_ns::DEV_ONLY) << "write: "<<i<<" : ";
        for (uint8_t j = 1; j <= size_shift; j++)
           logger()<<std::bitset<8>(calc.get_mapa().at(i).get_size() >> (size_shift - j) * 8);
        logger(log_ns::DEV_ONLY)<<std::endl;
#endif
    }
}

void ha_ns::read_shift_size(std::istream& stream_in, size_t& size) {
    uint8_t size_shift;
    stream_in >> size_shift;
    std::string buffer(size_shift, 0);
    stream_in.read(buffer.data(), size_shift);
    if (!stream_in)
        logger() << "WARN read "<<stream_in.gcount()<<" from "<<size_t(size_shift)<<std::endl;
    for (size_t i = 0; i < size_shift; i++) {
        uint8_t byte = static_cast<uint8_t>(buffer[i]);
        size |= static_cast<size_t>(byte) << (i * 8);
    }
    // logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<"size shift "<<size_t(size_shift)<<" buffer ";
    // for (const char& c : buffer)
    //     logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<size_t(c);
    // logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<" size " <<size<<std::endl;

}

void ha_ns::read_shift_size(std::istream& stream_in, std::map<size_t, std::vector<std::string>>& mapa, const size_t& model_size, const uint8_t num_byte, const uint8_t& node_size) {
    size_t read_bites = 0;
    if (stream_in) {
        int delta = num_byte + node_size;
        std::string str_tmp(model_size * delta, 0);
        stream_in.read(str_tmp.data(), str_tmp.size());
        read_bites = stream_in.gcount();
        if (read_bites != str_tmp.size()) throw "ERR";
        for (size_t i = 0; i < model_size; i++) {
            std::string sim = str_tmp.substr(i * delta, num_byte);
            size_t code = 0;
            for (size_t j = 0; j <node_size; j++){
                code <<= 8;
                code |= str_tmp[i * delta + j + num_byte];
            }
            mapa[code].push_back(sim);
            logger()<<"read " << sim << " with size: " << code;
            logger()<<std::endl;
        }
    }
}
