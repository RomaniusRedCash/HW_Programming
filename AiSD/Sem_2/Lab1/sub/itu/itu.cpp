#include "itu.h"
#include "itu_ha_classes.h"
#include <cstdint>

using namespace itu_ns;

uint8_t itu_ns::get_category(const int32_t& x) {
    for (uint8_t i = 0; i < 12; i++)
        if (std::abs(x) < (1 << i)) return i;
    throw "ERR";
}

void itu_ns::write_shift(sstrtobb& ssbb, const uint8_t& size, const int32_t& x) {
    uint8_t real_size = (size + 7) / 8;
    std::string str(real_size, 0);
    uint32_t real_x = (static_cast<uint32_t>(x) << (sizeof(int32_t) * 8 - size));
    for (uint8_t i = 0; i < real_size; i++) {
        str[i] = static_cast<char>((real_x >> (sizeof(int32_t) * 8 - (i + 1) * 8)) & 0xFF);
    }
    bytebit bb(size);
    bb << str;
    ssbb<<bb;
}

void itu_ns::read_shift(sstrtobb& ssbb, const uint8_t& size) {
    bytebit bb_tmp(size);
    ssbb>>bb_tmp;
    uint32_t x = 0;
    for (const char& c : bb_tmp.get_data()) {
        x<<=8;
        x |= static_cast<uint8_t>(c);
    }
    x>>=bb_tmp.get_size()%8;
}

bool itu_ns::try_read_code(sstrtobb& ssbb, bytebit& bb, const calculator& calc) {
    while (ssbb.get_data().size() * 8 - 8 + ssbb.get_buffer_sdvig_size() > bb.get_size()) {
        logger(log_ns::DEV_ONLY | log_ns::HARD_LVL)<<std::bitset<8>(ssbb.get_data()[bb.get_size()/8])<<"&"<<std::bitset<8>(0x80 >> bb.get_size() % 8)<<" pos " << bool(ssbb.get_data()[bb.get_size()/8] & (0x80 >> bb.get_size() % 8))  << std::endl;

        if (ssbb.get_data()[bb.get_size()/8] & (0x80 >> bb.get_size() % 8)) {
            bb.add_one();
        }
        else bb.add_null();
        if (calc.have_code(bb)) {
            logger(log_ns::NORMAL_LVL)<<"succes read ";
            for (const char& c : bb.get_data())
                logger(log_ns::NORMAL_LVL) << std::bitset<8>(c);
            logger(log_ns::NORMAL_LVL)<<" size " << bb.get_size() <<" : "<<calc[bb]<<std::endl;
            ssbb.sdvig(-bb.get_size());
            return true;
        }
        for (const char& c : bb.get_data())
            logger(log_ns::NORMAL_LVL) << std::bitset<8>(c);
        logger(log_ns::NORMAL_LVL)<<" size "<<bb.get_size()<<" not found"<<std::endl;
        if (bb.get_size() >= 16) {
            logger(log_ns::DEV_ONLY) << "already max size " << 16<<std::endl;
            return false;
        }
    }
    return false;
}

e_layer itu_ns::get_layer(const std::string& str) {
    if (str == "C") return itu_ns::e_layer::eCHROM;
    if (str == "nC") return itu_ns::e_layer::eNOCHROM;
    logger() << "ERROR! unknown layer.";
    throw "ERROR! unknown layer.";
}

void itu(std::istream& stream_in, std::ostream& stream_out, const itu_ns::e_layer& layer) {
    calculator* calc_DC, *calc_AC;
    switch (layer) {
        case eNOCHROM:
            calc_DC = new calculator_DC();
            calc_AC = new calculator_DC();
            break;
        case eCHROM:
            calc_DC = new calculator_CDC();
            calc_AC = new calculator_CDC();
        default:
            break;
    }
    sstrtobb ssbb;
    int32_t last_DC = 0;
    while (!stream_in.eof()) {
        std::vector<int32_t> ac_dc(64);
        stream_in.read(reinterpret_cast<char*>(ac_dc.data()), 64 * sizeof(int32_t));
        if(!stream_in.gcount()) break;
        int32_t now_C = ac_dc.front() - last_DC;
        uint8_t len = get_category(now_C);
        ssbb<<(*calc_DC)[len];
        last_DC = ac_dc.front();
        write_shift(ssbb, len, now_C);
        uint8_t pov_zer = 0;
        for (std::vector<int32_t>::iterator i = ac_dc.begin() + 1; i != ac_dc.end(); i++) {
            if(!*i) {
                pov_zer++;
                if (pov_zer > 11) break;
            }
            else {
                len = get_category(*i);
                len|=pov_zer << 4;
                ssbb<<(*calc_AC)[len];
                pov_zer=0;
                write_shift(ssbb, len, *i);
            }

        }
        if (pov_zer)
            ssbb<<char(0);
        ssbb.try_write(stream_out);
    }
    if(uint8_t sdvg = ssbb.get_buffer_sdvig_size()) {
        bytebit bb(8 - sdvg);
        bb << 0xFF;
        ssbb<<bb;
    }
    stream_out<<ssbb.get_data();
}
#define BUFFER_SIZE 1024
void de_itu(std::istream& stream_in, std::ostream& stream_out, const itu_ns::e_layer& layer) {
    calculator* calc_DC, *calc_AC;
    switch (layer) {
        case eNOCHROM:
            calc_DC = new calculator_DC();
            calc_AC = new calculator_DC();
            break;
        case eCHROM:
            calc_DC = new calculator_CDC();
            calc_AC = new calculator_CDC();
        default:
            break;
    }
    std::string buffer(BUFFER_SIZE, 0);
    size_t readed_bytes = BUFFER_SIZE;
    sstrtobb ssbb;
    while(!stream_in.eof()) {
        stream_in.read(buffer.data(), BUFFER_SIZE);
        readed_bytes = stream_in.gcount();
        buffer.resize(readed_bytes);
        ssbb<<buffer;
        uint32_t previos_DC = 0;
        while (ssbb.get_data().size()) {
            bytebit bb(0);
            if (try_read_code(ssbb, bb, *calc_DC)) {
                logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<"next buffer"<<std::endl;
                break;
            }
            uint8_t len  = (*calc_DC)[bb];
            uint32_t x;
            read_shift(ssbb, x);
            previos_DC += x;
            stream_out.write(reinterpret_cast<const char*>(&previos_DC), sizeof(previos_DC));
            uint8_t step = 0;
            while (ssbb.get_data().size() && step < 63) {
                bytebit bb(0);
                if (try_read_code(ssbb, bb, *calc_AC)) {
                    logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<"(maybe ERROR) next buffer"<<std::endl;
                    break;
                }
                uint8_t tuple_AC = (*calc_DC)[bb];
                if (!tuple_AC) {
                    for (; step < 63; step++)stream_out.put(0);
                    break;
                }
                uint8_t zeros = tuple_AC >> 4;
                len = tuple_AC & (0xFF >>4);
                for (uint8_t i = 0; i < zeros; i++) stream_out.put(0);
                read_shift(ssbb, x);
                stream_out.write(reinterpret_cast<const char*>(&x), sizeof(x));
            }
        }
    }
}
#undef BUFFER_SIZE
