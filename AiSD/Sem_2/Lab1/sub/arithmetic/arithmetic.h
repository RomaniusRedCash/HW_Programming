#pragma once

#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <limits>

namespace ar_nc {
    void maker(std::unordered_map<char, std::pair<size_t*, size_t>>& m_slovar, std::vector<std::pair<const char, std::pair<size_t*, size_t>>*>& v_slovar, const size_t& str_size);
}

std::stringstream arithmetic(const std::string& str_in);
std::string de_arithmetic(std::stringstream& ss_in);
