#include "compress_fun.h"

#define MASK_RLE 0x80

#define USLOVIE stream_in.get(byte) && !((len + 1) & MASK_RLE)
void RLE(std::istream& stream_in, std::ostream& stream_out) {
    // char C;
    // while (stream_in.get(C)) std::cout<<C;
    // std::cout<<std::endl;
    // stream_in.clear();
    // stream_in.seekg(0, std::ios::beg);
    char byte, redo_byte;
    while(stream_in.get(redo_byte)) {
        // std::cout<<int(redo_byte)<<std::endl;
        if (!stream_in.get(byte)) {
            stream_out.put(1);
            stream_out.write(&redo_byte, 1);
            break;
        }
        // std::cout<<int(byte)<<std::endl;

        uint8_t len = 2;
        if (redo_byte == byte) {
            while (USLOVIE) {
                if (byte != redo_byte) break;
                len++;
            }
            // std::cout<<int(len)<<'-'<<redo_byte<<' '<<byte<< ' ';
            len += MASK_RLE;
            stream_out.write(reinterpret_cast<char*>(&len), 1);
            stream_out.write(&redo_byte, 1);
        }
        else {
            std::string str_pool;
            str_pool += redo_byte;
            str_pool += byte;
            while (USLOVIE) {
                if (byte == redo_byte){
                    str_pool.pop_back();
                    break;
                }
                str_pool+=byte;
                redo_byte = byte;
                len++;
            }
            stream_out.write(reinterpret_cast<char*>(&len), 1);
            stream_out.write(str_pool.c_str(), str_pool.size());
            // std::cout<<int(len)<<'-'<<redo_byte<<' '<<byte<<' ';
        }
        if (stream_in.eof()) break;
        stream_in.clear();
        stream_in.seekg(-1, std::ios::cur);
    }
    // std::cout<<std::endl;
}
#undef USLOVIE

void from_RLE(std::istream& stream_in, std::ostream& stream_out) {
    char byte, len;
    while (stream_in.get(len)) {
        if (len & MASK_RLE) {
            len &= (~MASK_RLE);
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
