#pragma once

#include <string>
#include <vector>
#include "../bytebitstream.h"
#include "../bwt/bwt.h"
#include "../lz/lz77/lz77.h"
#include "../lz/lz78/lz78.h"
#include "../lz/lzw/lzw.h"
#include "../lz/lzss/lzss.h"
#include "../logger/logger.h"

void test(const std::string& str, const size_t& mc);
