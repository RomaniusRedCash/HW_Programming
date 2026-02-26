#include "sub_fun.h"
// #include <iostream>

#define MASK_RLE 0x80

#define USLOVIE stream_in.get(byte) && !((len + 1) & MASK_RLE)

void RLE(std::istream& stream_in, std::ostream& stream_out) {
    char byte, redo_byte;
    while(stream_in.get(redo_byte)) {
        if (!stream_in.get(byte)) {
            stream_out.put(1);
            stream_out.write(&redo_byte, 1);
            break;
        }

        uint8_t len = 2;
        if (redo_byte == byte) {
            while (USLOVIE) {
                if (byte != redo_byte) break;
                len++;
            }
            len += MASK_RLE;
            stream_out.write(reinterpret_cast<char*>(&len), 1);
            stream_out.write(&redo_byte, 1);
        }
        else {
            std::string str_pool;
            str_pool += redo_byte;
            str_pool += byte;
            redo_byte = byte;
            while (USLOVIE) {
                if (byte == redo_byte){
                    len--;
                    str_pool.pop_back();
                    break;
                }
                str_pool+=byte;
                redo_byte = byte;
                len++;
            }
            stream_out.write(reinterpret_cast<char*>(&len), 1);
            stream_out.write(str_pool.c_str(), str_pool.size());
        }
        if (stream_in.eof()) break;
        stream_in.clear();
        stream_in.seekg(-1 - (byte == redo_byte), std::ios::cur);
    }
}
#undef USLOVIE

void RLE2(std::istream& stream_in, std::ostream& stream_out) {
    char ch, ch_redo;

    while(stream_in.get(ch_redo)) {
        int len = 1;
        while (true) {
            if (!stream_in.get(ch) || ch != ch_redo) {
                stream_in.clear();
                if (len > 2) {
                    stream_in.seekg(-1, std::ios::cur);
                    len--;
                    len |= MASK_RLE;
                } else {
                    stream_in.seekg(-len, std::ios::cur);
                    len = 1;
                    break;
                }
            }
            len++;
            if ((len + 1) & MASK_RLE){
                stream_out.write(reinterpret_cast<char*>(&len), 1);
                stream_out.write(&ch_redo, 1);
                break;
            }
            ch_redo = ch;
        }
        if (!(len & MASK_RLE)) {
            std::string str_pool;
            str_pool+=ch_redo;
            while (stream_in.get(ch)) {
                size_t pos = stream_in.tellg();
                if (ch_redo == ch) {
                    if (stream_in.get(ch) && ch_redo == ch) {
                        str_pool.pop_back();
                        stream_in.clear();
                        stream_in.seekg(-3, std::ios::cur);
                        break;
                    }
                    // str_pool+=ch_redo;
                    ch = ch_redo;
                    stream_in.clear();
                    stream_in.seekg(-1, std::ios::cur);
                }
                ch_redo = ch;
                str_pool+=ch_redo;
                // if (((str_pool.size() + 1) & MASK_RLE )&& ch == ch_redo) {
                //     std::cout<<pos<<' '<<str_pool<<' '<<stream_in.tellg()<<std::endl;
                // }

                if ((str_pool.size() + 1) & MASK_RLE)
                    break;
            }
            len = str_pool.size();
            stream_out.write(reinterpret_cast<char*>(&len), 1);
            stream_out.write(str_pool.c_str(), len);
        }
    }
}

void from_RLE(std::istream& stream_in, std::ostream& stream_out) {
    char byte, len;
    while (stream_in.get(len)) {
        if (len & MASK_RLE) {
            len ^= MASK_RLE;
            if (stream_in.get(byte))
                for (char i = 0; i < len; i++)
                    stream_out << byte;
        }
        else {
            for (char i = 0; i < len && stream_in.get(byte); i++)
                stream_out << byte;
        }
    }
}

#undef MASK_RLE

void enwik8_to_enwik(std::istream& stream_in, std::ostream& stream_out) {
    char ch;
    for (size_t i = 0; i < 1E7 && stream_in.get(ch); i++) {
        stream_out<<ch;
    }
}

bool compare_f(std::istream& stream_in1, std::istream& stream_in2) {
    char ch1, ch2;
    do {
        stream_in1.get(ch1);
        stream_in2.get(ch2);

        if (ch1 != ch2) return false;
    } while (!stream_in1.eof() && !stream_in2.eof());

    if (stream_in1.eof() != stream_in2.eof()) return false;
    return true;
}

