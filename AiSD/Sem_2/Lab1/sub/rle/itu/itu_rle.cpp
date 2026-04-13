#include "itu_rle.h"
#include "itu_ha_classes.h"

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

void itu_rle(std::istream& stream_in, std::ostream& stream_out, const itu_ns::e_layer& layer) {
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

        // bytebit bb_len(4);
        int32_t now_C = ac_dc.front() - last_DC;
        uint8_t len = get_category(now_C);
        // bb_len << (len << 4);
        ssbb<<(*calc_DC)[len];
        last_DC = ac_dc.front();
        write_shift(ssbb, len, now_C);

        int32_t last_AC = 0;
        uint8_t pov_zer = 0;
        for (std::vector<int32_t>::iterator i = ac_dc.begin() + 1; i != ac_dc.end(); i++) {
            len = get_category(*i);
            uint16_t code = len;
            if (!*i) {
                if (pov_zer > 1 << 4) {

                }
            }
            ssbb<<(*calc_AC)[len];
            write_shift(ssbb, len, *i);
        }
        ssbb.try_write(stream_out);
    }
    stream_out<<ssbb.get_data();
}
