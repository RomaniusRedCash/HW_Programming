#include <ios>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>
#include <getopt.h>

#include "sub/sub_fun.h"
#include "sub/test/test.h"
#include "sub/rle/rle.h"
#include "sub/mtf/mtf.h"
#include "sub/ha/ha.h"

#include "sub/logger/logger.h"

enum eCOMMANDS : int;
std::map<std::string, eCOMMANDS> map_translater;

struct some_param {
    option opt;
    std::string name, desc;
    some_param(const char* name, const int& has_arg, int *flag, const int& val, const std::string desc, const eCOMMANDS& eC) :
    name(name), desc(desc)
    {
        opt = {name,has_arg,flag,val};
        map_translater[name] = eC;
    }
};

enum eCOMMANDS : int {
    eDEFAULT = 0,
    eBYTE,
    eCMPRE, eENWIKn7,
    eTEST,
    eLOGF, eLOGC,

    eRLE, eDERLE,
    eMTF, eDEMTF,
    eHA, eDEHA,
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

// NOTE: compress
    {"rle", no_argument, nullptr, 0, ":use RLE.", eRLE},
    {"mtf", no_argument, nullptr, 0, ":use MTF.", eMTF},
    {"ha", no_argument, nullptr, 0, ":use HA.", eHA},

// NOTE: decompress
    {"de-rle", no_argument, nullptr, 0, ":extract from RLE.", eDERLE},
    {"de-mtf", no_argument, nullptr, 0, ":extract from MTF.", eMTF},
    {"de-ha", no_argument, nullptr, 0, ":extract from HA.", eDEHA},

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
#ifdef DEBUG
    logger_demon::add_log_lvl(log_ns::DEV_ONLY);
#endif

    std::vector<option> long_opt;
    for (const some_param& i : v_someprm) long_opt.push_back(i.opt);

    int opt, long_idx;
    std::string str_inp;
    std::string str_out;
    std::string str_cmpr;
    int num_byte = 1;
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
                    default:
                        v_params.push_back(map_translater[long_opt[long_idx].name]);
                        break;
                }
                break;
            default:
                break;
        }

    if (str_inp.empty()) {
        std::cout<<"No input path."<<std::endl;
        // return 0;
    }
    if (str_out.empty()) str_out = str_inp + ".compr";

    #define TMP_FILE_NAME ".compr.tmp"
    std::fstream file_in(str_inp, std::ios::in);
    std::fstream file_tmp(TMP_FILE_NAME, std::ios::out | std::ios::in | std::ios::trunc);
    std::fstream file_out;

    if (!(file_in.is_open() && file_tmp.is_open())) {
        std::cout<<"ERROR! Some files can't openning or creating."<<std::endl;
        // return 0;
    }

    file_tmp << file_in.rdbuf();
    file_tmp.seekg(0, std::ios::beg);
    file_tmp.seekp(0, std::ios::beg);

    for (const eCOMMANDS& ec : v_params){
        file_out.open(str_out, std::ios::out | std::ios::in | std::ios::trunc);
        switch (ec) {


            case eENWIKn7:
                    enwik8_to_enwik(file_tmp, file_out);
                    break;

                // NOTE: compress
            case eRLE:
                std::cout<<"- start RLE"<<std::endl;
                RLE2(file_tmp, file_out, num_byte);
                std::cout<<"- stop RLE"<<std::endl;
                break;
            case eMTF:
                std::cout<<"- start MTF"<<std::endl;
                mtf(file_tmp, file_out, num_byte);
                std::cout<<"- stop MTF"<<std::endl;
                break;
            case eHA:
                std::cout<<"- start HA"<<std::endl;
                ha(file_tmp, file_out, num_byte);
                std::cout<<"- stop HA"<<std::endl;
                break;
                // NOTE: decompress
            case eDERLE:
                std::cout<<"- start deRLE"<<std::endl;
                from_RLE2(file_tmp, file_out, num_byte);
                std::cout<<"- stop deRLE"<<std::endl;
                break;
            case eDEMTF:
                std::cout<<"- start deMTF"<<std::endl;
                de_mtf(file_tmp, file_out, num_byte);
                std::cout<<"- stop deMTF"<<std::endl;
                break;




            case eTEST:
                // RLE_bit(file_tmp, file_out, 6, 6);
                test("123456", num_byte);
                break;

            default:
                break;
            }
            file_out.clear();
            file_out.seekg(0, std::ios::beg);
            file_tmp.close();
            file_tmp.open(TMP_FILE_NAME, std::ios::out | std::ios::in | std::ios::trunc);
            file_tmp << file_out.rdbuf();
            file_tmp.clear();
            file_tmp.seekg(0, std::ios::beg);
            file_out.close();
        }
    std::remove(TMP_FILE_NAME);
#undef TMP_FILE_NAME

    if (!str_cmpr.empty()) {
        std::cout<<"- start compare"<<std::endl;
        file_in.clear();
        file_in.seekg(0, std::ios::beg);
        std::fstream file_cmpr(str_cmpr, std::ios::in);
        if (compare_f(file_in, file_tmp)) std::cout<< "file is equal" << std::endl;
        else std::cout<< "file is not equal" << std::endl;
        file_cmpr.close();
        std::cout<<"- stop compare"<<std::endl;
    }
    file_in.close();

    return 0;
}



