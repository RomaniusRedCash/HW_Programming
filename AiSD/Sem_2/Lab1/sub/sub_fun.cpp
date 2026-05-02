#include "sub_fun.h"

void enwik8_to_enwik(std::istream& stream_in, std::ostream& stream_out) {
    char ch;
    for (size_t i = 0; i < 1E7 && stream_in.get(ch); i++) {
        stream_out<<ch;
    }
}
