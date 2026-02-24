#include "txt_compress.h"

void compress_txt(const std::string& name) {
    std::fstream file_in(name, std::ios::in);

    std::string new_name = {name.begin(), name.begin() + name.find_last_of('.')};
    std::fstream file_out(new_name + ".comptxt", std::ios::out);

    RLE(file_in, file_out);

    file_in.close();
    file_out.close();
}

void decompress_txt(const std::string& name) {
    std::fstream file_in(name, std::ios::in);

    std::string new_name = {name.begin(), name.begin() + name.find_last_of('.')};
    std::fstream file_out(new_name + ".txt", std::ios::out);

    from_RLE(file_in, file_out);

    file_in.close();
    file_out.close();
}
