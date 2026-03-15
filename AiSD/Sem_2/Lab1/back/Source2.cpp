#include <iostream>
#include <iomanip>
#include <vector>
#include <getopt.h>

struct some_param {
    option opt;
    std::string name, desc;
    some_param(const std::string& name, const int& has_arg, int *flag, const int& val, const std::string desc) :
        name(name), desc(desc)
    {
        opt = {name.c_str(),has_arg,flag,val};
    }
};

std::vector<some_param> v_someprm = {
    {"help", no_argument, nullptr, 'h', ":this page."},
    {"input", required_argument, nullptr, 'i', " [path]:path to input file."},
    {"output", required_argument, nullptr, 'o', " [path]:path to output file."},
    {"rle", no_argument, nullptr, 0, ":use RLE."},
    {"de-rle", no_argument, nullptr, 0, ":extract from rle."},
    {"byte", required_argument, nullptr, 0, "[num]:custom num of byte (default 1)."},
    {"compare", required_argument, nullptr, 0, " [path]:compare input file with file by path."},
    {"enwikn-to-enwik", no_argument, nullptr, 0, ":first 1E+7 byte from file"},
    {"test", no_argument, nullptr, 0, ":test"}
};


void print_help() {
    for (const some_param& i : v_someprm){
        std::cout<<std::left<<std::setw(25)<<i.name;

        size_t dvoet = i.desc.find_first_of(':');
        std::cout<<std::left<<std::setw(5)<<':';
        std::cout<< std::string(i.desc.begin() + dvoet + 1, i.desc.end()) <<std::endl;
    }
}

int main(const int argc, char* argv[]) {



    return 0;
}
