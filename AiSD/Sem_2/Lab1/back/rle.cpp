#include "rle.h"
// #include <memory>

elmnt::elmnt(const std::string& str, const int& size) {
    uint8_t  mask = 0xFF << (8-size%8);
    for (int i = 0; i <= size / 8; i++) {
        // if (size - i*8 < 8) data.push_back(str[i]&mask);
        // else data.push_back(str[i]);
        data.push_back(str[i] & ~(0xFF>>(size-8*i)));
    }
}

const std::string& elmnt::get_data() const {
    return data;
}

strelement::strelement(const int& mc) : mc(mc) {

}

void strelement::sdvig_str(std::string space, const int& sdvig_byte) {
    sdvig_buffer+=sdvig_byte;
    if (!sdvig_byte) return;
    for (std::string::iterator i = space.begin(); i != space.end() - 1; i++) {
        *i <<= sdvig_byte;
        i += *(i+1) >> (8 - sdvig_byte);
    }
    if (sdvig_buffer >= 8) {
        space.pop_back();
        sdvig_buffer-=8;
    }
}

strelement::strelement(const std::string& str, const int& start_point, const int& mc):
    str(str), mc(mc)
{
    sdvig_str(str, start_point);

    while (!this->str.empty()) {
        data.emplace_back(str, mc);
        sdvig_str(str, mc % 8);
    }
}

const elmnt& strelement::operator[](const size_t& iter) const {
    return data[iter];
}

void strelement::push_back(const elmnt& el) {
    data.push_back(el);
}

const std::string& strelement::burn_to_str(const int& ms) {
    size_t size = data.size();
    uint8_t real_size_char = sizeof(size);
    size <<= real_size_char - ms;
    const char* cptr = reinterpret_cast<const char*>(&size);
    for (uint8_t i = 0; i < ms/8; i++)
        str+=cptr[i];
    char c;
    int sdvig = ms%8;
    if (!sdvig)
        c = cptr[ms/8];
    for (const elmnt& i : data) {
        std::string tmp_str = i.get_data();
        while (!tmp_str.empty()) {
            c+=tmp_str[0] >> (8-sdvig);
            str+=c;
            sdvig_str(tmp_str, sdvig);
        }
    }

    return str;
}




void RLE_bit(std::istream& inp_stream, std::ostream& out_stream, const int& ms, const int& mc) {
    int num_char=mc*8;
    int c_readed = 0;
    std::string str(num_char, 0);
    do {
        inp_stream.read(str.data(), num_char);
        str.resize(c_readed = inp_stream.gcount());
        std::cout<<str<<" is read"<<std::endl;
        strelement stre =strelement(str,0,mc);
        std::cout<<"burning"<<std::endl;
        std::string tmp_str = stre.burn_to_str(ms);
        std::cout<<tmp_str<<" is writed"<<std::endl;

        out_stream.write(tmp_str.data(), tmp_str.size());
        return;
    } while(c_readed == num_char);
}








//
// elmnt::elmnt(const std::string& str, const int& start_point, const int& size) : size(size) {
//     int start_byte = start_point / 8;
//     int sdvig = start_point % 8;
//     for (int i = 0; i <= size / 8; i++) {
//         char c = (str[start_byte + i] << sdvig) + (str[start_byte + i + 1] >> (8 - sdvig));
//         data+=c;
//     }
// }
//
// const int& elmnt::get_size() const { return size; }
//
// strelement::strelement(const std::string& str, const int& start_point, const int& size) {
//
// }
//
// bool operator==(const elmnt& a, const elmnt& b) {
//     // if (a.get_size() != b.get_size()) return false;
//     // char iter = 1;
//     // for (int i = 0; i < a.size; i++) {
//     //     if ((a.data[i / 8] & iter) != (b.data[i / 8] & iter)) return false;
//     //     if (iter == 8) iter = 1;
//     //     else iter <<= 1;
//     // }
//     // return true;
//     return a.data == b.data;
// }
//
// RLE_comp::RLE_comp (const int& ms, const int& mc) : ms(ms), mc(mc) {
//     // while ((ms + mc * ++num_char % 8) != 0);
//     // num_char = (ms + mc * num_char)/8;
//     num_char=(mc*ms)/8+bool((mc*ms)%8);
//     mask <<= ms;
// }
//
// bool RLE_comp::convert_str(std::istream& inp_stream, std::ostream& out_stream){
//
//     std::string buffer(num_char, 0);
//     inp_stream.read(buffer.data(), num_char);
//
//
//
//
// }
