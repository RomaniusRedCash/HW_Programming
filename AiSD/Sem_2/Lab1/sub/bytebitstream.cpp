#include "bytebitstream.h"
#include "logger/logger.h"


//////////////////////////////////// ssbb_base ////////////////////////////////////

void ssbb_base::sdvig(int8_t delta) {
    logger(log_ns::DEV_ONLY | log_ns::HARD_LVL)<<"sdvig for "<<int(delta)<<std::endl;
    logger(log_ns::DEV_ONLY | log_ns::HARD_LVL)<<"last buffer "<<int(buffer_sdvig_size)<<" str now ";
    for (const char i : data)
        logger(log_ns::DEV_ONLY | log_ns::HARD_LVL)<<std::bitset<8>(i) << ' ';
    logger(log_ns::DEV_ONLY | log_ns::HARD_LVL)<<std::endl;

    if (delta > 0) {
        delta %= 8;
        if ((buffer_sdvig_size + delta - 1) /8 || !buffer_sdvig_size)
            data.push_back('\0');
        for (std::string::iterator i = data.end() - 1; i != data.begin(); i--) {
            *i>>=delta;
            *i&=0xFF>>delta;
            *i += *(i-1) << (8-delta);
        }
        data.front()>>=delta;
        data.front()&=0xFF>>delta;

        // if (buffer_sdvig_size >= 8) {
        //     // data.erase(data.begin());
        //     buffer_sdvig_size %= 8;
        // }
    } else if (delta < 0) {
        int8_t delta_tmp = -delta;
        if (delta_tmp >= 8) {
            if (delta_tmp / 8 > data.size()) throw "ERR";
            data.erase(data.begin(), data.begin() + delta_tmp / 8);
            delta_tmp%=8;
        }
        for (std::string::iterator i = data.begin(); i != data.end() - 1; i++) {
            *i<<=delta_tmp;
            *i +=(*(i+1) >> (8-delta_tmp)) & (0xFF >> (8-delta_tmp));
        }
        data.back()<<=delta_tmp;
        if (buffer_sdvig_size <= delta_tmp && buffer_sdvig_size)
            data.pop_back();
    }

    buffer_sdvig_size+=delta%8;
    buffer_sdvig_size = (buffer_sdvig_size + 8) % 8;
    logger(log_ns::DEV_ONLY | log_ns::HARD_LVL)<<"sdvig buffer now "<<int(buffer_sdvig_size);

    logger(log_ns::DEV_ONLY | log_ns::HARD_LVL)<<" str now ";
    for (const char i : data)
        logger(log_ns::DEV_ONLY | log_ns::HARD_LVL)<<std::bitset<8>(i) << ' ';
    logger(log_ns::DEV_ONLY | log_ns::HARD_LVL)<<std::endl;
}

ssbb_base::ssbb_base(const std::string& str) : data(str) {

}

const int8_t& ssbb_base::real_last() const {
    return buffer_sdvig_size;
}

const int8_t& ssbb_base::get_buffer_sdvig_size() const {
    return buffer_sdvig_size;
}

void ssbb_base::set_buffer_sdvig_size(const int8_t& sdvig) {
    buffer_sdvig_size = sdvig;
}

//////////////////////////////////// bytebit ////////////////////////////////////

bytebit::bytebit(const size_t& size) : size(size) {

}

bytebit& bytebit::operator<<(const char& c){
    logger(log_ns::DEV_ONLY)<<"try read "<<std::bitset<8>(c)<< " for size " << data.size() * 8 <<" from "<< size <<std::endl;
    if (data.size()*8 > size) throw "ERROR";
    int shift = size - data.size() * 8;
    shift = std::min(shift, 8);
    logger(log_ns::DEV_ONLY) << size - data.size()*8<< ' '<<std::bitset<8>(~(uint8_t(0xFF) >> shift))<<std::endl;
    data.push_back(c & ~(uint8_t(0xFF) >> shift));


    return *this;
}

const std::string& bytebit::get_data() const {
    return data;
}

const size_t& bytebit::get_size() const {
    return size;
}

//////////////////////////////////// sstrtobb ////////////////////////////////////
void sstrtobb::write(ssbb_base* ptr_ssbb) {
    sstrtobb ssbb = sstrtobb(ptr_ssbb->get_data());
    logger(log_ns::DEV_ONLY  | log_ns::HARD_LVL)<<"extract "<<&ssbb<<" data ";
    for (const char i : ssbb.get_data())
        logger(log_ns::DEV_ONLY | log_ns::HARD_LVL)<<std::bitset<8>(i) << ' ';
    logger(log_ns::DEV_ONLY | log_ns::HARD_LVL)<<std::endl;
    bytebit* bb = dynamic_cast<bytebit*>(ptr_ssbb);
    if (bb) {
        ssbb.buffer_sdvig_size = bb->get_size() % 8;
        logger(log_ns::DEV_ONLY | log_ns::HARD_LVL) << "is bb, size: " << bb->get_size() << std::endl;
    } else ssbb.set_buffer_sdvig_size(ptr_ssbb->get_buffer_sdvig_size());
    logger(log_ns::DEV_ONLY | log_ns::HARD_LVL)<<"before sdvig "<<int(buffer_sdvig_size)<<std::endl;

    if (buffer_sdvig_size) {
        ssbb.sdvig(buffer_sdvig_size);
        data.back() |= ssbb.data.front();
    }
    for (std::string::iterator i = ssbb.data.begin() + bool(buffer_sdvig_size); i != ssbb.data.end(); i++)
        data+=*i;
    buffer_sdvig_size=ssbb.buffer_sdvig_size%8;

    logger(log_ns::DEV_ONLY | log_ns::HARD_LVL)<<"after sdvig "<<int(buffer_sdvig_size)<<std::endl;
}

sstrtobb::sstrtobb() {};

sstrtobb::sstrtobb(const std::string& str) : ssbb_base(str) {

}

sstrtobb& sstrtobb::operator>>(bytebit& bb) {
    logger(log_ns::DEV_ONLY | log_ns::HARD_LVL)<<"read " << &bb<<" for size " << bb.get_size()<<std::endl;
    uint8_t size_in_byte = bb.get_size()/8;
    for (int i = 0; i <= size_in_byte; i++) {
        bb<<data[i];
    }
    logger(log_ns::DEV_ONLY | log_ns::HARD_LVL)<<&bb<<" now ";
    for (const char i : bb.get_data())
        logger(log_ns::DEV_ONLY | log_ns::HARD_LVL)<<std::bitset<8>(i) << ' ';
    logger(log_ns::DEV_ONLY | log_ns::HARD_LVL)<<std::endl;
    logger(log_ns::DEV_ONLY | log_ns::HARD_LVL)<<"cut str to " << int(size_in_byte)<<std::endl;
    data.erase(data.begin(), data.begin() + size_in_byte);
    sdvig(-(bb.get_size() % 8));

    return *this;
}

sstrtobb& sstrtobb::operator<<(bytebit bb) {
    write(&bb);
    return *this;
}
sstrtobb& sstrtobb::operator<<(sstrtobb ptr_ssbb) {
    write(&ptr_ssbb);
    return *this;
}

sstrtobb& sstrtobb::operator<<(const std::string& str) {
    if (!buffer_sdvig_size) {
        data+=str;
        return *this;
    }
    return *this << sstrtobb(str);
}

const std::string& sstrtobb::get_data() const {
    return data;
}

void sstrtobb::try_write(std::ostream& os) {
    if(!buffer_sdvig_size) {
        logger()<<"write on file ";
        for (const char& c : data)
            logger()<<std::bitset<8>(c);
        logger()<<std::endl;
        os.write(data.data(), data.size());
        data.clear();
    }
}
