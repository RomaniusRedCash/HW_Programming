#include "test.h"
#include <string>

void test(const std::string& str, const size_t& mc) {
    // bwt_ns::bwt_1(str, 1);

    // logger()<<"inp str '"<<str<<"' buffer is "<<5<<std::endl;
    // std::string str_tmp = lz77_ns::lz77_1(str, 5, 1);
    // for (size_t i = 0; i < str_tmp.size(); i+=3)
    //     logger()<<'{'<<size_t(str_tmp[i])<<", "<<size_t(str_tmp[i+1])<<", '"<<str_tmp[i+2]<<"'}, ";
    // logger()<<std::endl;
    // logger()<<"out "<<lz77_ns::de_lz77_1(str_tmp, 5, 1)<<std::endl;

    // logger()<<"inp str '"<<str<<"' buffer is "<<5<<std::endl;
    // std::string str_tmp = lz78_ns::lz78_1(str, 5, 1);
    // for (size_t i = 0; i < str_tmp.size(); i+=2)
    //     logger()<<'{'<<size_t(str_tmp[i])<<", '"<<str_tmp[i+1]<<"'}, ";
    // logger()<<std::endl;
    // logger()<<"out "<<lz78_ns::de_lz78_1(str_tmp, 5, 1)<<std::endl;

    logger()<<"inp str '"<<str<<"' buffer is "<<5<<std::endl;
    std::string str_tmp = lzw_ns::lzw_1(str, 5, 1);
    int x = 0;

    for (size_t i = 0; i < str_tmp.size(); i+=1+x) {
        if(i)x=1;
        size_t n = str_tmp[i+x];
        if(x) {
            n<<=8;
            n|=str_tmp[i];
        }
        logger()<<'{'<<n<<"}, ";
    }
    logger()<<std::endl;
    logger()<<"out "<<lzw_ns::de_lzw_1(str_tmp, 5, 1)<<std::endl;



    logger()<<"inp str '"<<str<<"' buffer is "<<5<<std::endl;
    std::string str_tmp2 = lzw_ns::lzw_2(str, 5, 1);
    x = 0;
    logger()<<std::endl;
    logger()<<"out "<<lzw_ns::de_lzw_2(str_tmp2, 5, 1)<<std::endl;

    logger() << "first "<<str_tmp.size() << " second " << str_tmp2.size()<<std::endl;

}
