#include "itu.h"
#include "itu_ha_classes.h"
#include <bitset>
#include <cstdint>

using namespace itu_ns;

uint8_t itu_ns::get_category(const int16_t& x) {
    uint16_t abs_x = std::abs(x);
    uint8_t category = 0;
    while (abs_x > 0) {
        abs_x >>= 1;
        category++;
    }
    return category;
}

void itu_ns::write_shift(sstrtobb& ssbb, const uint8_t& size, const int16_t& x) {
    if (size == 0) return;
    uint16_t x_tmp = x < 0 ? x - 1 : x;//static_cast<uint16_t>(x < 0 ? x - 1 : x);
    // x_tmp &= (1 << size) - 1;
    x_tmp &= 0xFFFF>>(sizeof(x) * 8 - size);
    logger(log_ns::DEV_ONLY) << "write "<<x<<" ("<<x_tmp<<") "<<' '<<std::bitset<16>(x_tmp << (sizeof(int16_t) * 8 - size))<<" with size "<<int(size)<<std::endl;
    uint8_t real_size = (size + 7) / 8;
    std::string str(real_size, 0);
    x_tmp = (x_tmp << (sizeof(int16_t) * 8 - size));
    for (uint8_t i = 0; i < real_size; i++) {
        str[i] = static_cast<char>((x_tmp >> (sizeof(int16_t) * 8 - (i + 1) * 8)) & 0xFF);
    }
    bytebit bb(size);
    bb << str;
    ssbb<<bb;
}

bool itu_ns::read_shift(sstrtobb& ssbb, const uint8_t& size, int16_t& x) {
    x = 0;
    if (!size) return true;
    if (ssbb.get_data().size() * 8 - (ssbb.get_buffer_sdvig_size() ? 8 - ssbb.get_buffer_sdvig_size() : 0) < size) return false;
    bytebit bb_tmp(size);
    ssbb>>bb_tmp;
    logger(log_ns::DEV_ONLY)<<"reading ";
    for (const uint8_t& c : bb_tmp.get_data()) {
        x<<=8;
        x |= c;
        logger(log_ns::DEV_ONLY)<<std::bitset<8>(c)<<' ';
    }
    x = static_cast<uint16_t>(x) >> (8 - (size % 8 == 0 ? 8 : size % 8));
    int16_t threshold = 1 << (size - 1);
    if (!(x & threshold)) x -= (1 << size) - 1;
    logger(log_ns::DEV_ONLY) << "read "<<std::bitset<16>(x) << '='<<x<<" with size "<<int(size)<<std::endl;
    return true;
}

bool itu_ns::try_read_code(sstrtobb& ssbb, bytebit& bb, const calculator& calc) {
    bb.clear();
    if (ssbb.get_data().empty()) return false;
    size_t sizessbb = ssbb.get_data().size() * 8;
    if (ssbb.get_buffer_sdvig_size()) {
        sizessbb -= 8;
        sizessbb += ssbb.get_buffer_sdvig_size();
    }
    while (sizessbb > bb.get_size()) {
        logger(log_ns::DEV_ONLY | log_ns::HARD_LVL)<<std::bitset<8>(ssbb.get_data()[bb.get_size()/8])<<"&"<<std::bitset<8>(0x80 >> bb.get_size() % 8)<<" pos " << bool(ssbb.get_data()[bb.get_size()/8] & (0x80 >> bb.get_size() % 8))  << std::endl;

        if (ssbb.get_data()[bb.get_size()/8] & (0x80 >> bb.get_size() % 8)) {
            bb.add_one();
        }
        else bb.add_null();
        if (calc.have_code(bb)) {
            logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<"succes read ";
            for (const char& c : bb.get_data())
                logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL) << std::bitset<8>(c);
            logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<" size " << bb.get_size() <<" : "<<(int)calc[bb]<<std::endl;
#ifndef DNDEBUG
            logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL) << "before sdvig ";
            for (uint8_t i = 0; i < std::min(size_t(2), ssbb.get_data().size()); i++)
                logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL) << std::bitset<8>(ssbb.get_data()[i]);
            logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<std::endl;
#endif
            ssbb.sdvig(-bb.get_size());
#ifndef DNDEBUG
            logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL) << "after sdvig ";
            for (uint8_t i = 0; i < std::min(size_t(2), ssbb.get_data().size()); i++)
                logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL) << std::bitset<8>(ssbb.get_data()[i]);
            logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<" size "<< sizessbb - bb.get_size() <<std::endl;
#endif
            return true;
        }
        for (const char& c : bb.get_data())
            logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL) << std::bitset<8>(c);
        logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<" size "<<bb.get_size()<<" not found"<<std::endl;
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
            calc_AC = new calculator_AC();
            break;
        case eCHROM:
            calc_DC = new calculator_CDC();
            calc_AC = new calculator_CAC();
            break;
        default:
            logger()<<"ERROR layer"<<std::endl;
            return;
            break;
    }
    sstrtobb ssbb;
    int16_t last_DC = 0;
    while (!stream_in.eof()) {
        std::vector<int16_t> ac_dc(64);
        stream_in.read(reinterpret_cast<char*>(ac_dc.data()), 64 * sizeof(int16_t));
        if(!stream_in.gcount()) break;
        int16_t now_C = ac_dc.front() - last_DC;
        uint8_t len = get_category(now_C);
#ifndef DNDEBUG

        sstrtobb ssbb_tmp;
        std::vector<uint8_t> v_pull;

        logger(log_ns::DEV_ONLY) << "write " << (int) ac_dc.front() << " DC len ";
        for (const char& c : (*calc_DC)[len].get_data()) {
            logger(log_ns::DEV_ONLY)<<std::bitset<8>(c);
        }
        logger(log_ns::DEV_ONLY) << " size " << (*calc_DC)[len].get_size() << std::endl;
        if ((*calc_DC)[(*calc_DC)[len]] != len)
            logger(log_ns::DEV_ONLY) << "ERROR! biman brocken!";
        ssbb_tmp << (*calc_DC)[len];
        write_shift(ssbb_tmp, len, now_C);
        v_pull.push_back(len);
#endif
        ssbb<<(*calc_DC)[len];
        last_DC = ac_dc.front();
        write_shift(ssbb, len, now_C);
        uint8_t pov_zer = 0;
        for (std::vector<int16_t>::iterator i = ac_dc.begin() + 1; i != ac_dc.end(); i++) {
            if(!*i) {
                pov_zer++;
            }
            else {
                while (pov_zer > 15) {
                    ssbb << (*calc_AC)[0xF0];
                    pov_zer -= 16;
#ifndef DNDEBUG
                    logger(log_ns::DEV_ONLY) << "write 16 zeros"<<std::endl;
                    ssbb_tmp << (*calc_AC)[0xF0];
#endif
                }
                len = get_category(*i);
                uint8_t sim = len | pov_zer << 4;
#ifndef DNDEBUG
                logger(log_ns::DEV_ONLY) << "zeros " << int(pov_zer) << " write AC step " << (int) (i - ac_dc.begin()) - pov_zer <<" sim "  << std::bitset<8>(sim) << " hac ";
                for (const char& c : (*calc_AC)[sim].get_data()) {
                    logger(log_ns::DEV_ONLY)<<std::bitset<8>(c);
                }
                logger(log_ns::DEV_ONLY)  << " size " << (*calc_AC)[len].get_size() << std::endl;
                if ((*calc_AC)[(*calc_AC)[sim]] != sim)
                    logger(log_ns::DEV_ONLY) << "ERROR! biman brocken!";
                ssbb_tmp << (*calc_AC)[sim];
                write_shift(ssbb_tmp, len, *i);
                v_pull.push_back(sim);
#endif
                ssbb<<(*calc_AC)[sim];
                pov_zer=0;
                write_shift(ssbb, len, *i);
            }
        }
        if (pov_zer) {
            pov_zer = 0;
#ifndef DNDEBUG
            logger(log_ns::DEV_ONLY)<<"fill zeros ";
            for (const char& c : (*calc_AC)[pov_zer].get_data())
                logger(log_ns::DEV_ONLY)<<std::bitset<8>(c);
            logger(log_ns::DEV_ONLY)<<" size " << (*calc_AC)[pov_zer].get_size() <<std::endl;
            ssbb_tmp << (*calc_AC)[pov_zer];
            v_pull.push_back(pov_zer);
#endif
            ssbb<<(*calc_AC)[pov_zer];
        }
        ssbb.try_write(stream_out);
#ifndef DNDEBUG // ###############################################################################
        uint8_t delta_step = 0;
        for (uint8_t substep = 0; substep < 64;) {
            bytebit bb(0);
            if (!try_read_code(ssbb_tmp, bb, *calc_DC))
                logger(log_ns::DEV_ONLY) << "ERROR! DC hac brocken!"<<std::endl;
            uint8_t len = (*calc_DC)[bb];
            if (len != v_pull[substep - delta_step])
                logger(log_ns::DEV_ONLY) << "ERROR! len DC brocken!"<<std::endl;
            if (!read_shift(ssbb_tmp, len, now_C))
                logger(log_ns::DEV_ONLY) << "ERROR! read delta DC brocken!"<<std::endl;
            // if (last_DC + now_C != ac_dc[substep])
            //     logger(log_ns::DEV_ONLY) << "ERROR! DC in not equal DC!"<<std::endl;
            for (substep++; substep < 64; substep++) {
                if (!try_read_code(ssbb_tmp, bb, *calc_AC))
                    logger(log_ns::DEV_ONLY) << "ERROR! AC hac brocken!"<<std::endl;
                uint8_t sim = (*calc_AC)[bb];
                if (sim != v_pull[substep - delta_step])
                    logger(log_ns::DEV_ONLY) << "ERROR! sim AC brocken! " << int(sim) << " != " <<int(v_pull[substep - delta_step])<<std::endl;
                uint8_t zeros = sim >> 4;
                len = sim & 0x0F;
                if (!read_shift(ssbb_tmp, len, now_C))
                    logger(log_ns::DEV_ONLY) << "ERROR! read AC brocken!"<<std::endl;
                substep+=zeros;
                delta_step+=zeros;
                if (now_C != ac_dc[substep])
                    logger(log_ns::DEV_ONLY) << "ERROR! AC not equal AC! " << now_C << " != " << ac_dc[substep] << std::endl;
                if(!sim) {
                    for (;substep < 64; substep++)
                        if (ac_dc[substep] != 0)
                            logger(log_ns::DEV_ONLY) << "ERROR! fill zeros!"<<std::endl;
                }
            }
        }
#endif // ###############################################################################
    }
    if(uint8_t sdvg = ssbb.get_buffer_sdvig_size()) {
        bytebit bb(8 - sdvg);
        bb << 0xFF;
        ssbb<<bb;
    }
    stream_out.write(ssbb.get_data().data(), ssbb.get_data().size());
}
#define BUFFER_SIZE 1024
void de_itu(std::istream& stream_in, std::ostream& stream_out, const itu_ns::e_layer& layer) {
    calculator* calc_DC, *calc_AC;
    switch (layer) {
        case eNOCHROM:
            calc_DC = new calculator_DC();
            calc_AC = new calculator_AC();
            break;
        case eCHROM:
            calc_DC = new calculator_CDC();
            calc_AC = new calculator_CAC();
        default:
            break;
    }
    std::string buffer(BUFFER_SIZE, 0);
    size_t readed_bytes = BUFFER_SIZE;
    sstrtobb ssbb;
    bytebit bb(0);
    int16_t previos_DC = 0;
#ifndef DNDEBUG
    std::fstream file("Y.tmp", std::ios::in | std::ios::binary);
#endif
    while(true) {
        if (!try_read_code(ssbb, bb, *calc_DC)) {
            if (stream_in.eof()) return;
            stream_in.read(buffer.data(), BUFFER_SIZE);
            buffer.resize(stream_in.gcount());
            ssbb<<buffer;
            if (!try_read_code(ssbb, bb, *calc_DC)) {
                logger(log_ns::DEV_ONLY)<<"unknown DC hac"<<std::endl;
                break;
            }
        }
        uint8_t len = (*calc_DC)[bb];
        int16_t x;
        while (!read_shift(ssbb, len, x)) {
            if (stream_in.eof()) throw "ERR";
            stream_in.read(buffer.data(), BUFFER_SIZE);
            buffer.resize(stream_in.gcount());
            ssbb<<buffer;
        }
        previos_DC+=x;
        stream_out.write(reinterpret_cast<const char*>(&previos_DC), sizeof(previos_DC));
#ifndef DNDEBUG
        logger(log_ns::DEV_ONLY) << "read DC " << previos_DC << " hac ";
        for (const char& c : bb.get_data()) {
            logger(log_ns::DEV_ONLY)<<std::bitset<8>(c);
        }
        logger(log_ns::DEV_ONLY)  << " size " << bb.get_size() << std::endl;
        file.read(reinterpret_cast<char*>(&x), sizeof(x));
        if (x != previos_DC)
            logger(log_ns::DEV_ONLY)  << "ERROR! DC not equal DC! " << (int) previos_DC << " != " << (int) x<< std::endl;
#endif
        for (uint8_t step = 0; step < 63; step++) {
            if (!try_read_code(ssbb, bb, *calc_AC)) {
                if (stream_in.eof()) throw "ERR";
                stream_in.read(buffer.data(), BUFFER_SIZE);
                buffer.resize(stream_in.gcount());
                ssbb<<buffer;
                if (!try_read_code(ssbb, bb, *calc_AC)) {
                    logger()<<"ERROR! read AC"<<std::endl;
                    throw "ERROR";
                }
            }
            uint8_t tuple_AC = (*calc_AC)[bb];
            x = 0;
            if (!tuple_AC) {
                logger(log_ns::DEV_ONLY)<<"fill zeros "<<63 - step<<std::endl;
                for (; step < 63; step++) {
                    stream_out.write(reinterpret_cast<const char*>(&x), sizeof(x));
#ifndef DNDEBUG
                    file.read(reinterpret_cast<char*>(&x), sizeof(x));
                    if (x != 0)
                        logger()<<"ERROR! fill zeros not zero"<<std::endl;
#endif
                }
                break;
            }
            uint8_t zeros = tuple_AC >> 4;
            len = tuple_AC & 0x0F;
#ifndef DNDEBUG
            logger(log_ns::DEV_ONLY)<<"zeros " << int(zeros) << " read AC step " << (int) step + 1<<" sim " << std::bitset<8>((*calc_AC)[bb]) << " hac ";
            for (const char& c : bb.get_data()) {
                logger(log_ns::DEV_ONLY)<<std::bitset<8>(c);
            }
            logger(log_ns::DEV_ONLY)  << " size " << bb.get_size() << std::endl;
#endif
            for (uint8_t i = 0; i < zeros; i++) {
                stream_out.write(reinterpret_cast<const char*>(&x), sizeof(x));
#ifndef DNDEBUG
                file.read(reinterpret_cast<char*>(&x), sizeof(x));
                if (x != 0)
                    logger()<<"ERROR! subfill zeros not zero"<<std::endl;
#endif
            }
            step+=zeros;
            while (!read_shift(ssbb, len, x)) {
                if (stream_in.eof()) throw "ERR";
                stream_in.read(buffer.data(), BUFFER_SIZE);
                buffer.resize(stream_in.gcount());
                ssbb<<buffer;
            }
#ifndef DNDEBUG
            int16_t X = 0;
            file.read(reinterpret_cast<char*>(&X), sizeof(X));
            if (x != X)
                logger()<<"ERROR! AC not equal AC! " << (int) x << " != " << (int) X <<std::endl;
#endif
            stream_out.write(reinterpret_cast<const char*>(&x), sizeof(x));
        }
    }
#ifndef DNDEBUG
    if(stream_in.eof()) logger(log_ns::DEV_ONLY)<<"eof"<<std::endl;
    file.close();
#endif

}
#undef BUFFER_SIZE
