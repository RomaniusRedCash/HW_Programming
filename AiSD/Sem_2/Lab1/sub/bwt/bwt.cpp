#include "bwt.h"

extern uint8_t num_byte;

using namespace bwt_ns;

std::string bwt_ns::bwt_1(const std::string& str) {
    std::string str_out;
    size_t size = str.size()/num_byte;
    // if (size > (1 << sizeof(uint8_t) * 8) - 1) throw "err";
    std::vector<std::string> matrix(size);
    for (size_t i = 0; i < size; i++)
        for (size_t j = i; j < size + i; j++)
            matrix[i]+=str.substr((j%size)*num_byte, num_byte);
    std::sort(matrix.begin(), matrix.end());
    std::vector<std::string>::iterator it = std::find(matrix.begin(), matrix.end(), str);
    uint32_t pos = (it - matrix.begin());
    str_out.resize(sizeof(pos));
    std::memcpy(str_out.data(), &pos, sizeof(pos));
    for (const std::string& s : matrix)
        str_out+=s.substr(s.size() - num_byte, num_byte);

#ifndef NDEBUG
    logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL) << "str in "<<str<<std::endl;
    logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL) << "str out "<<str_out<<" - " <<size_t(str_out.front())<<std::endl;
    logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL) << "matrix: "<<std::endl;
    for (const std::string& s : matrix)
        logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL) << s << std::endl;
#endif

    return str_out;
}

std::string bwt_ns::bwt_2(const std::string& str) {
    if (str.empty()) return "";
    size_t str_size = str.size();
    std::vector<size_t> ways(str_size);
    std::iota(ways.begin(), ways.end(), 0);
    std::sort(ways.begin(), ways.end(), [&](size_t a, size_t b) {
        if (a == b) return false;
        for (size_t i = 0; i < str_size; ) {
            size_t len = std::min(str_size - a, str_size - b);
            auto cmp = std::string_view(str.data() + a, len) <=> std::string_view(str.data() + b, len);
            if (cmp != 0) return cmp < 0;
            a = (a + len) % str_size;
            b = (b + len) % str_size;
            i += len;
        }
        return false;
    });
    uint32_t bwt_pos = 0;
    std::string str_out;
    str_out.reserve(str_size + sizeof(bwt_pos));
    str_out.resize(sizeof(bwt_pos));
    for (size_t i = 0; i < str_size; i++) {
        if (ways[i] == 0) bwt_pos = i;
        str_out += str[(ways[i] + str_size - 1) % str_size];
    }
    std::memcpy(str_out.data(), &bwt_pos, sizeof(bwt_pos));
    return str_out;
}
std::string bwt_ns::bwt_3(const std::string& str) {
    if (str.empty()) return "";
    const size_t n = str.size();
    const size_t num_symbols = n / num_byte;
    std::string double_str = str + str;
    const char* d_ptr = double_str.data();
    std::vector<size_t> ways(num_symbols);
    for(size_t i = 0; i < num_symbols; ++i) ways[i] = i * num_byte;

    std::sort(ways.begin(), ways.end(), [&](size_t a, size_t b) {
        int cmp = memcmp(d_ptr + a, d_ptr + b, n);
        if (cmp != 0) return cmp < 0;
        return a < b;
    });
    uint32_t bwt_pos = 0;
    std::string res;
    res.reserve(n + 4);
    res.resize(4);
    for (size_t i = 0; i < num_symbols; ++i) {
        if (ways[i] == 0) bwt_pos = (uint32_t)i;
        size_t last_idx = (ways[i] + n - num_byte) % n;
        res.append(str.data() + last_idx, num_byte);
    }
    std::memcpy(res.data(), &bwt_pos, 4);
    return res;
}

std::string bwt_ns::de_bwt_1(const std::string& s_bwt, const uint8_t& bwt_pos) {
    size_t size = s_bwt.size() / num_byte;
    if (size > (1 << sizeof(uint8_t) * 8) - 1) throw "err";
    std::vector<std::string> v_pull(size, std::string(size, 0));
    std::map<std::string, uint8_t> map_bwt;
    for (int i = 0; i < size; i+=num_byte) map_bwt[s_bwt.substr(i, num_byte)]++;

    size_t pos = 0;
    for (std::pair<const std::string, uint8_t>& i : map_bwt)
        for (uint8_t j = 0; j < i.second; j++) {
            std::string& str = v_pull[pos];
            std::string::iterator startit = str.begin() + (size - 1) * num_byte;
            str.replace(startit, startit + num_byte, i.first);
            pos++;
        }
    for (uint8_t posx = 1; posx < size; posx++) {
        for (pos = 0; pos < size; pos++) {
            std::string& str = v_pull[pos];
            std::string::iterator startit = str.begin() + (size - 1 - posx) * num_byte;
            str.replace(startit, startit + num_byte, std::string_view(s_bwt.data() + pos * num_byte, num_byte));
        }
        std::sort(v_pull.begin(), v_pull.end());
    }
    logger(log_ns::DEV_ONLY)<< "table BWT: "<< std::endl;
    for (const std::string str : v_pull)
        logger(log_ns::DEV_ONLY)<< str << std::endl;
    logger(log_ns::DEV_ONLY)<< "====" << std::endl;
    return v_pull[bwt_pos];
}

std::string bwt_ns::de_bwt_2(const std::string& s_bwt, const uint32_t& bwt_pos) {
    const size_t n = s_bwt.size();
    if (n == 0) return "";
    std::string res;
    res.reserve(n);
    std::array<size_t, std::numeric_limits<uint8_t>::max()> counts = {0};
    std::vector<size_t> char_order(n);

    for (size_t i = 0; i < n; ++i) {
        uint8_t c = static_cast<uint8_t>(s_bwt[i]);
        char_order[i] = counts[c];
        counts[c]++;
    }
    std::array<size_t, std::numeric_limits<uint8_t>::max()> start_pos;
    size_t sum = 0;
    for (int i = 0; i < start_pos.size(); i++) {
        start_pos[i] = sum;
        sum += counts[i];
    }
#ifndef NDEBUG
    logger(log_ns::DEV_ONLY) << "perehodi:" << std::endl;
    for (size_t i = 0; i < n; ++i) {
        uint8_t c = static_cast<uint8_t>(s_bwt[i]);
        size_t next_idx = start_pos[c] + char_order[i];
        logger(log_ns::DEV_ONLY)<< c << "_" << char_order[i] << " -> "<< s_bwt[next_idx] << "_" << char_order[next_idx]<< std::endl;
    }
#endif
    size_t curr = bwt_pos;
    for (size_t i = 0; i < n; ++i) {
        uint8_t c = static_cast<uint8_t>(s_bwt[curr]);
        res += c;
        curr = start_pos[c] + char_order[curr];
    }
    std::reverse(res.begin(), res.end());
    return res;
}

std::string bwt_ns::de_bwt_3(const std::string& s_bwt, const uint32_t& bwt_pos) {
    size_t size = s_bwt.size() / num_byte;
    // if (size > (1 << sizeof(uint8_t) * 8) - 1) throw "err";
    std::string str_out(s_bwt);
    std::vector<std::pair<std::string_view, uint32_t>> v_pool(size);
    std::map<std::string_view, uint32_t> mapa;
    for (size_t i = 0; i < size; i++) {
        std::string_view s_tmp(s_bwt.data() + i * num_byte, num_byte);
        uint32_t& num_s = mapa[s_tmp];
        v_pool[i] = {s_tmp, num_s};
        num_s++;
    }
    std::map<std::string_view, uint32_t>::iterator it;
    uint32_t less = 0;
    for (it = mapa.begin();it != mapa.end(); it++)
        less+=it->second;
    for (--it; it != mapa.begin(); it--)
        it->second = less-=it->second;
    it->second = 0;
#ifndef NDEBUG
    logger(log_ns::DEV_ONLY) << "table #1:" << std::endl;
    for (const std::pair<std::string_view, uint8_t>& i : v_pool)
        logger(log_ns::DEV_ONLY) << i.first << " : " << (int) i.second << std::endl;
    logger(log_ns::DEV_ONLY) << "table #2:" << std::endl;
    for (const std::pair<std::string_view, uint8_t>& i : mapa)
        logger(log_ns::DEV_ONLY) << i.first << " : " << (int) i.second << std::endl;
#endif

    logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL) << "WORK: "<<std::endl;
    std::pair<std::string_view, uint32_t>& pr = v_pool[bwt_pos];
    logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL) << size_t(bwt_pos) << " : " << pr.first << std::endl;
    str_out.replace((size - 1) * num_byte, num_byte, pr.first);
    less = pr.second + mapa[pr.first];
    for (uint8_t i = 1; i < size; i++) {
        logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL) << (size - i - 1) * num_byte << " : " << size_t(pr.second) << " + " << size_t(mapa[pr.first]) << " = " << less << v_pool[less].first << std::endl;
        pr = v_pool[less];
        str_out.replace((size - i - 1) * num_byte, num_byte, pr.first);
        less = pr.second + mapa[pr.first];
    }
    return str_out;
}

std::string bwt_ns::de_bwt_4(const std::string& s_bwt, const uint32_t& bwt_pos) {
    size_t total_bytes = s_bwt.size();
    size_t num_symbols = total_bytes / num_byte;
    if (num_symbols == 0) return "";

    // 1. Считаем вхождения каждого "символа"
    // Используем unordered_map, чтобы не тормозить на поиске в дереве
    std::unordered_map<std::string_view, uint32_t> counts;
    std::vector<uint32_t> char_occurrence_index(num_symbols);

    for (size_t i = 0; i < num_symbols; ++i) {
        std::string_view sym(s_bwt.data() + i * num_byte, num_byte);
        char_occurrence_index[i] = counts[sym]++;
    }

    // 2. Создаем отсортированный список уникальных символов для кумулятивной суммы
    std::vector<std::string_view> alphabet;
    alphabet.reserve(counts.size());
    for (auto const& [sym, count] : counts) {
        alphabet.push_back(sym);
    }
    std::sort(alphabet.begin(), alphabet.end());

    // 3. Вычисляем стартовые позиции в отсортированном массиве (C[c])
    std::unordered_map<std::string_view, uint32_t> start_pos;
    uint32_t current_sum = 0;
    for (auto const& sym : alphabet) {
        start_pos[sym] = current_sum;
        current_sum += counts[sym];
    }

    // 4. Восстанавливаем строку с конца
    std::string res;
    res.resize(total_bytes);

    uint32_t curr = bwt_pos;
    for (size_t i = 0; i < num_symbols; ++i) {
        std::string_view sym(s_bwt.data() + curr * num_byte, num_byte);

        // Записываем в обратном порядке
        size_t write_pos = (num_symbols - 1 - i) * num_byte;
        std::memcpy(res.data() + write_pos, sym.data(), num_byte);

        // Переход: T[i] = C[L[i]] + Rank(L[i], i)
        curr = start_pos[sym] + char_occurrence_index[curr];
    }

    return res;
}

std::string bwt_ns::de_bwt_0(const std::string& str) {
    uint32_t bwt_pos = 0;
    std::string_view str_bwtpos(str.data(), sizeof(bwt_pos));
    std::memcpy(&bwt_pos, str_bwtpos.data(), sizeof(bwt_pos));
    if (num_byte > 1)
        return bwt_ns::de_bwt_4(str.substr(sizeof(bwt_pos), str.size() - sizeof(bwt_pos)), bwt_pos);
    return bwt_ns::de_bwt_2(str.substr(sizeof(bwt_pos), str.size() - sizeof(bwt_pos)), bwt_pos);
}

void bwt(std::istream& stream_in, std::ostream& stream_out) {
    size_t window_buffer_size = lz::get_window(sub_commands.front(), 1<<16);
    // if (window_buffer_size == 0) window_buffer_size = (1<<16);
    std::string buffer;
    // if (num_byte > 1)
        buffer.resize(window_buffer_size - sizeof(uint32_t));
    // else buffer.resize(window_buffer_size - sizeof(uint32_t));
    while (stream_in) {
        stream_in.read(buffer.data(), buffer.size());
        buffer.resize(stream_in.gcount());
        if (buffer.empty()) return;
        std::string new_str;
        if (num_byte > 1) new_str = bwt_ns::bwt_3(buffer);
        else new_str = bwt_ns::bwt_2(buffer);
        stream_out.write(new_str.data(),new_str.size());
    }
}
void de_bwt(std::istream& stream_in, std::ostream& stream_out) {
    size_t window_buffer_size = lz::get_window(sub_commands.front(), 1<<16);
    // if (window_buffer_size == 0) window_buffer_size = (1<<16);
    std::string buffer;
    // if (num_byte > 1)
        // buffer.resize((1 << (sizeof(uint8_t) * num_byte * 8)) - num_byte);
    // else
        buffer.resize(window_buffer_size);
    while (stream_in) {
        stream_in.read(buffer.data(), buffer.size());
        buffer.resize(stream_in.gcount());
        if (buffer.empty()) return;
        std::string new_str = bwt_ns::de_bwt_0(buffer);
        stream_out.write(new_str.data(),new_str.size());
    }
}
