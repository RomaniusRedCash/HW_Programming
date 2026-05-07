#pragma once

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
#ifndef NDEBUG
#include "sub/test/test.h"
#endif
#include "sub/rle/rle.h"
#include "sub/mtf/mtf.h"
#include "sub/ha/ha.h"
#include "sub/bwt/bwt.h"
#include "sub/lz/lzw/lzw.h"
#include "sub/lz/lzss/lzss.h"
#include "sub/itu/itu.h"

#include "sub/logger/logger.h"

enum eCOMMANDS : int {
    eDEFAULT = 0,
    eBYTE,
    eENWIKn7,
    eTEST,
    eLOGF, eLOGC, eLOGN, eLOGH,

    eRLE, eDERLE,
    eMTF, eDEMTF,
    eHA, eDEHA,
    eBWT, eDEBWT,
    eLZW, eDELZW,
    eLZSS, eDELZSS,

    eITU, eDEITU
};

struct some_param;

struct some_param {
    option opt;
    std::string name, desc;
    bool add_to_list = false;
    eCOMMANDS ec;
    some_param(const char* name, const int& has_arg, int *flag, const int& val, const std::string desc, const eCOMMANDS& ec, const bool& add_to_list) :
    name(name), desc(desc), ec(ec), add_to_list(add_to_list)
    {
        opt = {name,has_arg,flag,val};
    }
};

const some_param v_someprm[] = {
    {"help", no_argument, nullptr, 'h', ":this page.", eDEFAULT, false},
    {"input", required_argument, nullptr, 'i', " [path]:path to input file.", eDEFAULT, false},
    {"output", required_argument, nullptr, 'o', " [path]:path to output file.", eDEFAULT, false},
    {"byte", required_argument, nullptr, 0, "[num]:custom num of byte (default 1).", eBYTE, false},
    {"enwikn-to-enwik", no_argument, nullptr, 0, ":first 1E+7 byte from file", eENWIKn7, true},
#ifndef NDEBUG
    {"test", no_argument, nullptr, 0, ":test", eTEST, true},
#endif
    {"log-file", no_argument, nullptr, 0, ":log in file", eLOGF, false},
    {"log-console", no_argument, nullptr, 0, ":log in console", eLOGC, false},
    {"log-n", no_argument, nullptr, 0, ":log in normal lvl", eLOGN, false},
    {"log-h", no_argument, nullptr, 0, ":log in hard lvl", eLOGH, false},
// NOTE: compress
    {"rle", no_argument, nullptr, 0, ":use RLE.", eRLE, true},
    {"mtf", no_argument, nullptr, 0, ":use MTF.", eMTF, true},
    {"ha", no_argument, nullptr, 0, ":use HA.", eHA, true},
    {"bwt", optional_argument, nullptr, 0, ":use BWT.", eBWT, true},
    {"lzw", optional_argument, nullptr, 0, ":use LZW.", eLZW, true},
    {"lzss", optional_argument, nullptr, 0, ":use LZSS.", eLZSS, true},

// NOTE: decompress
    {"de-rle", no_argument, nullptr, 0, ":extract from RLE.", eDERLE, true},
    {"de-mtf", no_argument, nullptr, 0, ":extract from MTF.", eDEMTF, true},
    {"de-ha", no_argument, nullptr, 0, ":extract from HA.", eDEHA, true},
    {"de-bwt", optional_argument, nullptr, 0, "[buffer_size]:extract from BWt.", eDEBWT, true},
    {"de-lzw", optional_argument, nullptr, 0, "[style,slovar_size]:extract from LZW. \"f\" -- write slovar, \"d\" -- not write (default).", eDELZW, true},
    {"de-lzss", optional_argument, nullptr, 0, "[buffer_size]:extract from LZSS.", eDELZSS, true},

    {"itu", required_argument, nullptr, 0, "[C/nC]:compress JPEG layer.", eITU, true},
    {"de-itu", required_argument, nullptr, 0, "[C/nC]:decompress JPEG layer.", eDEITU, true},

};
