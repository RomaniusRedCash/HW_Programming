#include "test.h"

void test(const std::string& str, const size_t& mc) {
    // bwt_ns::bwt_1(str, 1);

    // logger()<<"inp str '"<<str<<"' buffer is "<<5<<std::endl;
    // std::string str_tmp = lz77_ns::lz77_1(str, 5, 1);
    // for (size_t i = 0; i < str_tmp.size(); i+=3)
    //     logger()<<'{'<<size_t(str_tmp[i])<<", "<<size_t(str_tmp[i+1])<<", '"<<str_tmp[i+2]<<"'}, ";
    // logger()<<std::endl;
    // logger()<<"out "<<lz77_ns::de_lz77_1(str_tmp, 5, 1)<<std::endl;

    logger()<<"inp str '"<<str<<"' buffer is "<<5<<std::endl;
    std::string str_tmp = lz78_ns::lz78_1(str, 5, 1);
    for (size_t i = 0; i < str_tmp.size(); i+=2)
        logger()<<'{'<<size_t(str_tmp[i])<<", "<<size_t(str_tmp[i+1])<<"'}, ";
    logger()<<std::endl;
    logger()<<"out "<<lz78_ns::de_lz78_1(str_tmp, 5, 1)<<std::endl;

}
