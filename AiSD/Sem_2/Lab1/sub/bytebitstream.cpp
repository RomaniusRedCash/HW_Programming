#include "bytebitstream.h"
#include "logger/logger.h"
#include <cstdint>

//////////////////////////////////// ssbb_base ////////////////////////////////////

void ssbb_base::sdvig_l(const uint8_t& delta) {
    logger(log_ns::DEV_ONLY)<<"sdvig_l for "<<int(delta)<<std::endl;
    logger(log_ns::DEV_ONLY)<<"str now ";
    for (const char i : data)
        logger(log_ns::DEV_ONLY)<<std::bitset<8>(i) << ' ';
    logger(log_ns::DEV_ONLY)<<std::endl;

    if (delta > 8) throw "ERROR";
    buffer_sdvig_size+=delta;
    for (std::string::iterator i = data.begin(); i != data.end() - 1; i++) {
        *i<<=delta;
        *i +=(*(i+1) >> (8-delta)) & (0xFF >> (8-delta));
    }
    data.back()<<=delta;
    if (buffer_sdvig_size >= 8) {
        data.pop_back();
        buffer_sdvig_size -= 8;
    }

    logger(log_ns::DEV_ONLY)<<"str now ";
    for (const char i : data)
        logger(log_ns::DEV_ONLY)<<std::bitset<8>(i) << ' ';
    logger(log_ns::DEV_ONLY)<<std::endl;
}

void ssbb_base::sdvig_r(const uint8_t& delta) {
    logger(log_ns::DEV_ONLY)<<"sdvig_r for "<<int(delta)<<std::endl;
    logger(log_ns::DEV_ONLY)<<"str now ";
    for (const char i : data)
        logger(log_ns::DEV_ONLY)<<std::bitset<8>(i) << ' ';
    logger(log_ns::DEV_ONLY)<<std::endl;

    if (delta > 8) throw "ERROR";
    data.resize(data.size() + 1);
    buffer_sdvig_size-=delta;
    for (std::string::iterator i = data.end() - 1; i != data.begin(); i--) {
        *i>>=delta;
        *i&=0xFF>>delta;
        *i += *(i-1) << (8-delta);
    }
    data.front()>>=delta;
    data.front()&=0xFF>>delta;
    if (buffer_sdvig_size <= -8) {
        data.erase(data.begin());
        buffer_sdvig_size += 8;
    }

    logger(log_ns::DEV_ONLY)<<"str now ";
    for (const char i : data)
        logger(log_ns::DEV_ONLY)<<std::bitset<8>(i);
    logger(log_ns::DEV_ONLY)<<std::endl;

}

ssbb_base::ssbb_base(const std::string& str) : data(str) {

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

sstrtobb::sstrtobb() {};

sstrtobb::sstrtobb(const std::string& str) : ssbb_base(str) {

}

sstrtobb& sstrtobb::operator>>(bytebit& bb) {
    logger(log_ns::DEV_ONLY)<<"read " << &bb<<" for size " << bb.get_size()<<std::endl;
    uint8_t size_in_byte = bb.get_size()/8;
    for (int i = 0; i <= size_in_byte; i++) {
        bb<<data[i];
    }
    logger(log_ns::DEV_ONLY)<<&bb<<" now ";
    for (const char i : bb.get_data())
        logger(log_ns::DEV_ONLY)<<std::bitset<8>(i) << ' ';
    logger(log_ns::DEV_ONLY)<<std::endl;

    // if (size_in_byte)
    logger(log_ns::DEV_ONLY)<<"cut str to " << int(size_in_byte)<<std::endl;
    data.erase(data.begin(), data.begin() + size_in_byte);
    sdvig_l(bb.get_size() % 8);

    return *this;
}

sstrtobb& sstrtobb::operator<<(bytebit bb) {

    logger(log_ns::DEV_ONLY)<<"extract "<<&bb<<" data ";
    for (const char i : bb.get_data())
        logger(log_ns::DEV_ONLY)<<std::bitset<8>(i) << ' ';
    logger(log_ns::DEV_ONLY)<<std::endl;

    logger(log_ns::DEV_ONLY)<<"sdvig now "<<int(buffer_sdvig_size)<<std::endl;
    if (buffer_sdvig_size) {
        bb.sdvig_r(buffer_sdvig_size);
        data.back() += bb.data.front();
    }

    for (std::string::iterator i = bb.data.begin() + bool(buffer_sdvig_size); i != bb.data.end(); i++)
        data+=*i;

    buffer_sdvig_size+=bb.get_size();
    buffer_sdvig_size %= 8;

    return *this;
}

sstrtobb& sstrtobb::operator<<(const std::string& str) {
    data+=str;
    return *this;
}

const std::string& sstrtobb::get_data() const {
    return data;
}

void sstrtobb::try_write(std::ostream& os) {
    if(!buffer_sdvig_size) {
        logger()<<"write on file ";
        logger().write(data.data(), data.size());
        logger()<<std::endl;
        os.write(data.data(), data.size());
        data.clear();
    }
}
