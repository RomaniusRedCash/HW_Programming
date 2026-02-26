#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
// #include <filesystem>
#include "sub_fun.h"
// #include "txt_compress.h"

#include <getopt.h>


static struct option long_opt[] = {
    {"help", no_argument, nullptr, 'h'},
    {"input", required_argument, nullptr, 'i'},
    {"output", required_argument, nullptr, 'o'},
    {"rle", no_argument, nullptr, 0},
    {"rle2", no_argument, nullptr, 0},
    {"de-rle", no_argument, nullptr, 0},
    {"compare", required_argument, nullptr, 0},
    {"enwikn-to-enwik", no_argument, nullptr, 0},
    {0,0,0,0}
};

enum eCOMMANDS{
    eRLE, eRLE2, eDERLE, eCMPRE, eENWIKn7
};

std::map<std::string, eCOMMANDS> map_translater = {
    {"rle", eRLE},
    {"rle2", eRLE2},
    {"de-rle", eDERLE},
    {"compare", eCMPRE},
    {"enwikn-to-enwik", eENWIKn7}
};

void print_help() {
    std::cout<<"-h or --help:\tthis page"<<std::endl;
}

int main(const int argc, char* argv[]) {
    int opt, long_idx;
    std::string str_inp;
    std::string str_out;
    std::string str_cmpr;
    std::vector<eCOMMANDS> v_params;
    while ((opt = getopt_long(argc, argv, "hi:o:", long_opt, &long_idx)) != -1)
        switch (opt) {
            case 'h':
                print_help();
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
        return 0;
    }
    if (str_out.empty()) str_out = str_inp + ".compr";

    #define TMP_FILE_NAME ".compr.tmp"
    std::fstream file_in(str_inp, std::ios::in);
    std::fstream file_tmp(TMP_FILE_NAME, std::ios::out | std::ios::in | std::ios::trunc);
    std::fstream file_out;

    if (!(file_in.is_open() && file_tmp.is_open())) {
        std::cout<<"ERROR! Some files can't openning or creating."<<std::endl;
        return 0;
    }

    file_tmp << file_in.rdbuf();
    file_tmp.seekg(0, std::ios::beg);
    file_tmp.seekp(0, std::ios::beg);

    for (const eCOMMANDS& ec : v_params){
        file_out.open(str_out, std::ios::out | std::ios::in | std::ios::trunc);
        switch (ec) {
            case eRLE:
                RLE(file_tmp, file_out);
                break;
            case eRLE2:
                RLE2(file_tmp, file_out);
                break;
            case eDERLE:
                from_RLE(file_tmp, file_out);
                break;
            case eENWIKn7:
                    enwik8_to_enwik(file_tmp, file_out);
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
        file_in.clear();
        file_in.seekg(0, std::ios::beg);
        std::fstream file_cmpr(str_cmpr, std::ios::in);
        if (compare_f(file_in, file_tmp)) std::cout<< "file is equal" << std::endl;
        else std::cout<< "file is not equal" << std::endl;
    }
    file_in.close();

    return 0;
}



