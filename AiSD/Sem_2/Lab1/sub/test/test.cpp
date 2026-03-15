#include "test.h"

void test(const std::string& str, const size_t& mc) {
    logger()<<"in "<<str<<std::endl;
    sstrtobb ssbb_in(str);
    std::vector<bytebit> v(str.size() * 8 / mc, mc);
    logger()<<"pool size "<<v.size()<<std::endl;
    for (bytebit& i : v) ssbb_in >> i;
    for (bytebit& i : v) logger()<<&i<<": "<<i.get_data()<<" size = "<<i.get_size()<<std::endl;
    sstrtobb ssbb_out;
    for (bytebit& i : v) ssbb_out << i;
    logger()<<"out "<<ssbb_out.get_data()<<std::endl;
}
