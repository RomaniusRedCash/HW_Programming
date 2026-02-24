#include <iostream>
#include <string>
#include <map>
#include "compress_fun.h"
#include "txt_compress.h"



namespace cCOMMANDS {
    const char TXT_COMP[] = "--txt-comp";
    const char TXT_DECOMP[] = "--txt-decomp";
}

enum eCOMMANDS{
    HELP, TXT_COMPRESS, TXT_DECOMPRESS, BIN
};
std::map<std::string, eCOMMANDS> map_of_command = {
    {"-h", eCOMMANDS::HELP},
    {"--help", eCOMMANDS::HELP},
    {cCOMMANDS::TXT_COMP, eCOMMANDS::TXT_COMPRESS},
    {cCOMMANDS::TXT_DECOMP, eCOMMANDS::TXT_DECOMPRESS}
};

void print_help() {
    std::cout<<"-h or --help:\tthis page"<<std::endl;
    std::cout<<cCOMMANDS::TXT_COMP<<" [path]:\tcompress txt"<<std::endl;
    std::cout<<"--bin [path]:\tcompress binary file"<<std::endl;
    std::cout<<"--raw [path]:\tcompress photo"<<std::endl;
}

void print_error(){
    std::cout<<"Some error. Use -h for help."<<std::endl;
}



int main(const int argc, const char* argv[]) {
    if (argc > 4 || !map_of_command.contains(argv[1])) print_error();

    else if (argc == 1 || map_of_command[argv[1]] == eCOMMANDS::HELP) print_help();

    else switch(map_of_command[argv[1]]){
        case eCOMMANDS::TXT_COMPRESS:
            compress_txt(argv[2]);
            break;
        case eCOMMANDS::TXT_DECOMPRESS:
            decompress_txt(argv[2]);
            break;
        default: print_error; break;
    }



    // std::fstream file_out2(name + "_out.txt", std::ios::in);
    // from_RLE(file_out2);

    return 0;
}
