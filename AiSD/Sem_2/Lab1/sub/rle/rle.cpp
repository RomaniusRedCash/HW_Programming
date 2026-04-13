#include "rle.h"

#define MASK_RLE 0x80

#define BUFFER_SIZE 1024
#define SUB_BUFFER_SIZE 130
void RLE(std::istream& stream_in, std::ostream& stream_out) {
    std::string buffer(BUFFER_SIZE, 0);
    while (stream_in) {
        stream_in.read(buffer.data(), BUFFER_SIZE);
        size_t read_bytes = stream_in.gcount();
        if (read_bytes == 0) break;
        int len = 0;
        for (size_t chunk = 0; chunk < read_bytes; chunk+=len) {
            if (chunk + SUB_BUFFER_SIZE > read_bytes && !stream_in.eof()){
                stream_in.clear();
                stream_in.seekg(-(read_bytes-chunk), std::ios::cur);
                break;
            }
            size_t str_len = std::min<size_t>(SUB_BUFFER_SIZE, read_bytes-chunk);
            std::string_view sub_buffer(buffer.data() + chunk, str_len);
            len = sub_buffer.find_first_not_of(sub_buffer.front());
            if (len == -1) len = str_len;
            if (len > 2) {
                if (len & MASK_RLE)
                    len = MASK_RLE;
                stream_out.put(uint8_t((len - 1) | MASK_RLE));
                stream_out.put(sub_buffer.front());

                continue;
            }
            for(len = 0; len < MASK_RLE && len < str_len - 2; len++)
                if (sub_buffer[len] == sub_buffer[len + 1] && sub_buffer[len] == sub_buffer[len+2])
                    break;
            if (str_len < 3) {
                len = str_len;
            }
            stream_out.put(uint8_t(len - 1));
            stream_out.write(sub_buffer.data(), len);
        }
    }
}

void RLE2(std::istream& stream_in, std::ostream& stream_out, const int& num_byte) {
    std::string buffer(BUFFER_SIZE, 0);
    while (stream_in) {
        stream_in.read(buffer.data(), BUFFER_SIZE);
        size_t read_bytes = stream_in.gcount();
        if (read_bytes == 0) break;
        if (read_bytes < num_byte*3) {
            stream_out.put(0);
            stream_out.write(buffer.data(), read_bytes);
            return;
        }
        int len = 0;
        for (size_t chunk = 0; chunk < read_bytes; chunk+=len*num_byte) {
            if (chunk + SUB_BUFFER_SIZE > read_bytes && !stream_in.eof()){
                stream_in.clear();
                stream_in.seekg(-(read_bytes-chunk), std::ios::cur);
                break;
            }
            size_t str_len = std::min<size_t>(SUB_BUFFER_SIZE, read_bytes-chunk);
            std::string_view sub_buffer(buffer.data() + chunk, str_len);
            if(str_len < num_byte) {
                stream_out.put(0);
                stream_out.write(sub_buffer.data(), str_len);
                return;
            }
            len = (find_first_not_of_str(sub_buffer, sub_buffer.substr(0,num_byte))-sub_buffer.begin())/num_byte;
            if (len == -1) len = str_len;
            if (len > 2) {
                if (len & MASK_RLE)
                    len = MASK_RLE;
                stream_out.put(uint8_t((len - 1) | MASK_RLE));
                stream_out.write(sub_buffer.data(), num_byte);

                logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<"r: "<<len<<' ';
                logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL).write(sub_buffer.data(), num_byte);
                logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<std::endl;

                continue;
            }
            for(len = 0; len < MASK_RLE && len * num_byte + 2< str_len; len++)
                if (sub_buffer.substr(len * num_byte, num_byte) == sub_buffer.substr((len + 1) * num_byte, num_byte) && sub_buffer.substr(len * num_byte, num_byte) == sub_buffer.substr((len + 2) * num_byte, num_byte))
                    break;
            if (str_len < 3 || str_len < SUB_BUFFER_SIZE) {
                len = str_len;
            }
            stream_out.put(uint8_t(len - 1));
            stream_out.write(sub_buffer.data(), len * num_byte);

            logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<"nr: "<<len<<' ';
            logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL).write(sub_buffer.data(), len * num_byte);
            logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<std::endl;
        }
    }
}
#undef BUFFER_SIZE
#undef SUB_BUFFER_SIZE

void from_RLE(std::istream& stream_in, std::ostream& stream_out) {
    char byte;
    uint8_t len;
    while (stream_in.get(byte)){
        len = byte;
        if (len & MASK_RLE) {
            len ^= MASK_RLE;
            if (stream_in.get(byte))
                for (uint8_t i = 0; i <= len; i++){
                    stream_out << byte;
                }
        }
        else {
            len++;
            std::string str(len, 0);
            stream_in.read(str.data(), len);
            stream_out.write(str.data(), len);
        }
    }

}

void from_RLE2(std::istream& stream_in, std::ostream& stream_out, const int& num_byte) {
    char byte;
    uint8_t len;
    while (stream_in.get(byte)){
        len = byte;
        std::string str;
        if (len & MASK_RLE) {
            len ^= MASK_RLE;
            str.resize(num_byte, 0);
            if (stream_in.read(str.data(), num_byte))
                for (uint8_t i = 0; i <= len; i++){
                    stream_out.write(str.data(), num_byte);
                    logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<byte;
                }
        }
        else {
            len++;
            str.resize(len * num_byte, 0);
            stream_in.read(str.data(), len * num_byte);
            stream_out.write(str.data(), stream_in.gcount());
            logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<std::endl<<int(len)<<std::endl;
            logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<str;
        }
    }

}
#undef MASK_RLE

std::string_view::const_iterator find_first_not_of_str(const std::string_view& str1, const std::string_view& str2) {
    for(size_t i = 0; i < str1.size(); i+=str2.size()) {
        std::string_view str_w(str1.data() + i, str2.size());
        if (str_w != str2) return str1.begin() + i;
    }
    return str1.end();
}
