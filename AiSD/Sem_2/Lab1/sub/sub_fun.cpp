#include "sub_fun.h"

void enwik8_to_enwik(std::istream& stream_in, std::ostream& stream_out) {
    char ch;
    for (size_t i = 0; i < 1E7 && stream_in.get(ch); i++) {
        stream_out<<ch;
    }
}

bool compare_f(std::istream& stream_in1, std::istream& stream_in2) {
    char ch1, ch2;
    do {
        stream_in1.get(ch1);
        stream_in2.get(ch2);

        if (ch1 != ch2) return false;
    } while (!stream_in1.eof() && !stream_in2.eof());

    if (stream_in1.eof() != stream_in2.eof()) return false;
    return true;
}
