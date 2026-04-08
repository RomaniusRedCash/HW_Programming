#include "mtf.h"
#include <sstream>

#define BUFFER_SIZE 1024 * num_byte
void mtf(std::istream& stream_in, std::ostream& stream_out, const int& num_byte) {
    std::deque<std::string> v_alph;
    std::vector<std::string> v_alph_const;
    std::string buffer_in(BUFFER_SIZE,0);
    while (stream_in) {
        stream_in.read(buffer_in.data(), BUFFER_SIZE);
        size_t read_bytes = stream_in.gcount();
        logger()<<"read str: "<<buffer_in<<" size = "<<read_bytes<<std::endl;
        if (read_bytes == 0) break;
        for (int i = 0; i < read_bytes / num_byte; i++) {
            std::string_view now_sim(buffer_in.data() + i * num_byte, num_byte);
            logger(log_ns::NORMAL_LVL)<<"read: "<<now_sim;
            std::deque<std::string>::iterator iter = std::find(v_alph.begin(), v_alph.end(), now_sim);
            if (iter == v_alph.end()) {
                v_alph_const.emplace_back(now_sim);
                v_alph.emplace_front(now_sim);
                stream_out<<now_sim;
                logger(log_ns::NORMAL_LVL)<<" write new: "<<now_sim<<std::endl;
            } else {
                stream_out<<v_alph_const[iter - v_alph.begin()];
                logger(log_ns::NORMAL_LVL)<<" write: "<<v_alph_const[iter - v_alph.begin()]<<std::endl;
                if (iter != v_alph.begin()) {
                    v_alph.erase(iter);
                    v_alph.emplace_front(now_sim);
                }
            }
#ifndef NDEBUG
            logger(log_ns::DEV_ONLY | log_ns::HARD_LVL) << "slovar const ";
            for (const std::string& s : v_alph_const) logger(log_ns::DEV_ONLY | log_ns::HARD_LVL) << s<< ' ';
            logger(log_ns::DEV_ONLY | log_ns::HARD_LVL) << std::endl << "slovar ";
            for (const std::string& s : v_alph) logger(log_ns::DEV_ONLY | log_ns::HARD_LVL) << s<< ' ';
            logger(log_ns::DEV_ONLY | log_ns::HARD_LVL) << std::endl;
#endif
        }
    }
}

void de_mtf(std::istream& stream_in, std::ostream& stream_out, const int& num_byte) {
    std::deque<std::string> v_alph;
    std::vector<std::string> v_alph_const;
    std::string buffer(BUFFER_SIZE,0);
    while (stream_in) {
        std::stringstream buffer_out;
        stream_in.read(buffer.data(), BUFFER_SIZE);
        size_t read_bytes = stream_in.gcount();
        logger(log_ns::NORMAL_LVL)<<"read str: "<<buffer<<" size = "<<read_bytes<<std::endl;
        if (read_bytes == 0) break;
        for (int i = 0; i < read_bytes / num_byte; i++) {
            std::string_view now_sim(buffer.data() + i * num_byte, num_byte);
            logger(log_ns::NORMAL_LVL)<<"read: "<<now_sim;
            std::vector<std::string>::iterator iter = std::find(v_alph_const.begin(), v_alph_const.end(), now_sim);
            if (iter == v_alph_const.end()) {
                v_alph_const.emplace_back(now_sim);
                v_alph.emplace_front(now_sim);
                stream_out<<now_sim;
                logger(log_ns::NORMAL_LVL)<<" write new: "<<now_sim<<std::endl;
            } else {
                const size_t pos = iter - v_alph_const.begin();
                stream_out<<v_alph[pos];
                logger(log_ns::NORMAL_LVL)<<" write: "<<v_alph[pos]<<std::endl;
                if (pos != 0) {
                    const std::string sim = v_alph[pos];
                    v_alph.erase(v_alph.begin() + pos);
                    v_alph.emplace_front(sim);
                }
            }
#ifndef NDEBUG
            logger(log_ns::DEV_ONLY | log_ns::HARD_LVL) << "slovar const ";
            for (const std::string& s : v_alph_const) logger(log_ns::DEV_ONLY | log_ns::HARD_LVL) << s<< ' ';
            logger(log_ns::DEV_ONLY | log_ns::HARD_LVL) << std::endl << "slovar ";
            for (const std::string& s : v_alph) logger(log_ns::DEV_ONLY | log_ns::HARD_LVL) << s<< ' ';
            logger(log_ns::DEV_ONLY | log_ns::HARD_LVL) << std::endl;
#endif
        }
    }
}
#undef BUFFER_SIZE
