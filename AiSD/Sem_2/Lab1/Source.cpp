#include <ios>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
// #include <filesystem>
#include "sub_fun.h"
// #include "txt_compress.h"
#include <iomanip>
#include <sstream>

#include <getopt.h>

std::vector<std::pair<option, std::string>> v_data_com = {
    {{"help", no_argument, nullptr, 'h'}, ":this page."},
    {{"input", required_argument, nullptr, 'i'}, " [path]:path to input file."},
    {{"output", required_argument, nullptr, 'o'}, " [path]:path to output file."},
    {{"rle", no_argument, nullptr, 0}, ":use RLE."},
    {{"de-rle", no_argument, nullptr, 0}, ":extract from rle."},
    {{"byte", required_argument, nullptr, 0}, "[num]:custom num of byte (default 1)."},
    {{"compare", required_argument, nullptr, 0}, " [path]:compare input file with file by path."},
    {{"enwikn-to-enwik", no_argument, nullptr, 0}, ":first 1E+7 byte from file"},
};

enum eCOMMANDS{
   eDEFAULT = 0, eRLE, eDERLE, eBYTE, eCMPRE, eENWIKn7
};

std::map<std::string, eCOMMANDS> map_translater = {
    {"rle", eRLE},
    {"de-rle", eDERLE},
    {"byte", eBYTE},
    {"compare", eCMPRE},
    {"enwikn-to-enwik", eENWIKn7}
};

void print_help() {
    // std::cout<<"-h or --help:\tthis page"<<std::endl;
    for (std::pair<option, std::string>& i : v_data_com){
        std::stringstream ss;
        ss<<' ';
        if (i.first.val != 0)
            ss<<'-'<<char(i.first.val) << ", ";
        size_t dvoet = i.second.find_first_of(':');
        ss <<"--"<< i.first.name << std::string(i.second.begin(), i.second.begin() + dvoet);
        std::cout<<std::left<<std::setw(25)<<ss.str();
        std::cout<<std::left<<std::setw(5)<<':';
        std::cout<< std::string(i.second.begin() + dvoet + 1, i.second.end())<<std::endl;
    }
}

int main(const int argc, char* argv[]) {
    std::vector<option> long_opt;
    for (std::pair<option, std::string>& i : v_data_com) long_opt.push_back(i.first);




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
                std::cout<<"- start RLE"<<std::endl;
                RLE2(file_tmp, file_out, num_byte);
                std::cout<<"- stop RLE"<<std::endl;
                break;
            case eDERLE:
                std::cout<<"- start deRLE"<<std::endl;
                from_RLE2(file_tmp, file_out, num_byte);
                std::cout<<"- stop deRLE"<<std::endl;
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



