#include <ios>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <iomanip>
#include <getopt.h>

#include "sub/sub_fun.h"
#include "sub/test/test.h"
#include "sub/rle/rle.h"
#include "sub/mtf/mtf.h"
#include "sub/ha/ha.h"
#include "sub/lz/lzw/lzw.h"
#include "sub/lz/lzss/lzss.h"

#include "sub/logger/logger.h"

enum eCOMMANDS : int;
bimap<std::string, eCOMMANDS> map_translater;

struct some_param {
    option opt;
    std::string name, desc;
    some_param(const char* name, const int& has_arg, int *flag, const int& val, const std::string desc, const eCOMMANDS& eC) :
    name(name), desc(desc)
    {
        opt = {name,has_arg,flag,val};
        map_translater.insert(name, eC);
    }
};

enum eCOMMANDS : int {
    eDEFAULT = 0,
    eBYTE,
    eCMPRE, eENWIKn7,
    eTEST,
    eLOGF, eLOGC, eLOGN, eLOGH,

    eRLE, eDERLE,
    eMTF, eDEMTF,
    eHA, eDEHA,
    eLZW, eDELZW,
    eLZSS, eDELZSS,
};

std::vector<some_param> v_someprm = {
    {"help", no_argument, nullptr, 'h', ":this page.", eDEFAULT},
    {"input", required_argument, nullptr, 'i', " [path]:path to input file.", eDEFAULT},
    {"output", required_argument, nullptr, 'o', " [path]:path to output file.", eDEFAULT},
    {"byte", required_argument, nullptr, 0, "[num]:custom num of byte (default 1).", eBYTE},
    {"compare", required_argument, nullptr, 0, " [path]:compare input file with file by path.", eCMPRE},
    {"enwikn-to-enwik", no_argument, nullptr, 0, ":first 1E+7 byte from file", eENWIKn7},
    {"test", no_argument, nullptr, 0, ":test", eTEST},
    {"log-file", no_argument, nullptr, 0, ":log in file", eLOGF},
    {"log-console", no_argument, nullptr, 0, ":log in console", eLOGC},
    {"log-n", no_argument, nullptr, 0, ":log in normal lvl", eLOGN},
    {"log-h", no_argument, nullptr, 0, ":log in hard lvl", eLOGH},

// NOTE: compress
    {"rle", no_argument, nullptr, 0, ":use RLE.", eRLE},
    {"mtf", no_argument, nullptr, 0, ":use MTF.", eMTF},
    {"ha", no_argument, nullptr, 0, ":use HA.", eHA},
    {"lzw", optional_argument, nullptr, 0, ":use LZW.", eLZW},
    {"lzss", optional_argument, nullptr, 0, ":use LZSS.", eLZSS},

// NOTE: decompress
    {"de-rle", no_argument, nullptr, 0, ":extract from RLE.", eDERLE},
    {"de-mtf", no_argument, nullptr, 0, ":extract from MTF.", eDEMTF},
    {"de-ha", no_argument, nullptr, 0, ":extract from HA.", eDEHA},
    {"de-lzw", optional_argument, nullptr, 0, ":extract from LZW.", eDELZW},
    {"de-lzss", optional_argument, nullptr, 0, ":extract from LZSS.", eDELZSS},

};

void print_help() {
    for (const some_param& i : v_someprm){
        std::cout<<"\t--"<<std::left<<std::setw(20)<<i.name;

        size_t dvoet = i.desc.find_first_of(':');
        std::cout<<std::left<<std::setw(10)<<':';
        std::cout<< std::string(i.desc.begin() + dvoet + 1, i.desc.end()) <<std::endl;
    }
}

int main(const int argc, char* argv[]) {
#ifndef NDEBUG
    logger_demon::add_log_lvl(log_ns::DEV_ONLY);
#endif

    std::vector<option> long_opt;
    for (const some_param& i : v_someprm) long_opt.push_back(i.opt);

    int opt, long_idx;
    std::string str_inp;
    std::string str_out;
    std::string str_cmpr;
    uint8_t num_byte = 1;
    uint8_t buffer_size_lz = 8;
    std::vector<eCOMMANDS> v_params;
    while ((opt = getopt_long(argc, argv, "hi:o:", &long_opt.front(), &long_idx)) != -1)
        switch (opt) {
            case 'h':
                print_help();
                return 0;
                break;
            case 'i':
                str_inp = optarg;
                break;
            case 'o':
                str_out = optarg;
                break;
            case 0:
                switch (map_translater[long_opt[long_idx].name]) {

                    case eCMPRE:
                        str_cmpr = optarg;
                        break;
                    case eBYTE:
                        num_byte = std::stoi(optarg);
                        if (num_byte == 0) {
                            std::cout<<"ochen smeshno"<<std::endl;
                            return 0;
                        }
                        break;
                    case eLOGF:
                        logger_demon::open_logfile("log.log");
                        logger_demon::add_log_lvl(log_ns::FILE_LVL);
                        break;
                    case eLOGC:
                        logger_demon::add_log_lvl(log_ns::CONS_LVL);
                        break;
                    case eLOGH:
                        logger_demon::add_log_lvl(log_ns::HARD_LVL);
                    case eLOGN:
                        logger_demon::add_log_lvl(log_ns::NORMAL_LVL);
                        break;

                    case eLZW:
                    case eDELZW:
                    case eLZSS:
                    case eDELZSS:
                        if (optarg != nullptr)
                            buffer_size_lz = std::stoi(optarg);
                    default:
                        v_params.push_back(map_translater[long_opt[long_idx].name]);
                        break;
                }
                break;
            default:
                break;
        }

    if (str_inp.empty()) std::cout<<"No input path."<<std::endl;
    if (str_out.empty()) str_out = str_inp + ".compr";
    std::fstream file_in(str_inp, std::ios::in | std::ios::binary);
    std::stringstream ss_tmp1,ss_tmp2;
    std::stringstream *p_ss_tmp1 = &ss_tmp1, *p_ss_tmp2 = &ss_tmp2;
    std::fstream file_out;
    if (file_in.is_open())
        ss_tmp1 << file_in.rdbuf();
    else {
        std::cout<<"ERROR! Some files can't openning or creating."<<std::endl;
        return 1;
    }
    for (const eCOMMANDS& ec : v_params) {
        p_ss_tmp2->str(std::string());
        p_ss_tmp2->clear();
        p_ss_tmp1->clear();
        p_ss_tmp1->seekg(0,std::ios::beg);
        switch (ec) {
            case eENWIKn7:
                enwik8_to_enwik(*p_ss_tmp1, *p_ss_tmp2);
                break;

                // NOTE: compress
            case eRLE:
                start_algorithm(map_translater[ec], [&]{RLE2(*p_ss_tmp1, *p_ss_tmp2, num_byte);});
                break;
            case eMTF:
                start_algorithm(map_translater[ec], [&]{mtf(*p_ss_tmp1, *p_ss_tmp2, num_byte);});
                break;
            case eHA:
                start_algorithm(map_translater[ec], [&]{ha(*p_ss_tmp1, *p_ss_tmp2, num_byte);});
                break;
            case eLZW:
                start_algorithm(map_translater[ec], [&]{lzw(*p_ss_tmp1, *p_ss_tmp2, num_byte, buffer_size_lz);});
                break;
            case eLZSS:
                start_algorithm(map_translater[ec], [&]{lzss(*p_ss_tmp1, *p_ss_tmp2, num_byte, buffer_size_lz);});
                break;

                // NOTE: decompress
            case eDERLE:
                start_algorithm(map_translater[ec], [&]{from_RLE2(*p_ss_tmp1, *p_ss_tmp2, num_byte);});
                break;
            case eDEMTF:
                start_algorithm(map_translater[ec], [&]{de_mtf(*p_ss_tmp1, *p_ss_tmp2, num_byte);});
                break;
            case eDEHA:
                start_algorithm(map_translater[ec], [&]{de_ha(*p_ss_tmp1, *p_ss_tmp2, num_byte);});
                break;
            case eDELZW:
                start_algorithm(map_translater[ec], [&]{de_lzw(*p_ss_tmp1, *p_ss_tmp2, num_byte, buffer_size_lz);});
                break;
            case eDELZSS:
                start_algorithm(map_translater[ec], [&]{de_lzss(*p_ss_tmp1, *p_ss_tmp2, num_byte, buffer_size_lz);});
                break;


            case eTEST:
                test("abacabacabadaca", num_byte);
                break;
            default:
                break;
        }
        std::swap(p_ss_tmp1, p_ss_tmp2);
    }
    file_out.open(str_out, std::ios::out);
    if (!file_out.is_open()) {
        logger() << "ERROR! Can't create out file."<<std::endl;
        return 1;
    } else {
        p_ss_tmp1->clear();
        p_ss_tmp1->seekg(0,std::ios::beg);
        file_out<<p_ss_tmp1->rdbuf();
    }
    if (!str_cmpr.empty()) {
        file_in.clear();
        file_in.seekg(0,std::ios::beg);
        std::fstream file_cmpr(str_cmpr, std::ios::in | std::ios::binary);
        if (!file_cmpr.is_open()) {
            logger()<<"ERROR! Can't open file for compare."<<std::endl;
            return 1;
        }
        start_algorithm("compare", [&]{logger() << (compare_f(file_cmpr, file_in) ? "file is equal" : "file is not equal") << std::endl;});
        file_cmpr.close();
    }
    file_in.close();

    return 0;
}



