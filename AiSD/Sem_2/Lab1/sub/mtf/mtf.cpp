#include "mtf.h"
#include <sstream>

#define BUFFER_SIZE 1024 * num_byte
void mtf(std::istream& stream_in, std::ostream& stream_out, const int& num_byte) {
    std::deque<std::string> v_alph;
    std::vector<std::string> v_alph_const;
    std::string buffer_in(BUFFER_SIZE,0);


    while (stream_in) {
        std::stringstream buffer_out;
        stream_in.read(buffer_in.data(), BUFFER_SIZE);
        size_t read_bytes = stream_in.gcount();
        logger()<<"read str: "<<buffer_in<<" size = "<<read_bytes<<std::endl;
        if (read_bytes == 0) break;
        for (int i = 0; i < read_bytes / num_byte; i++) {
            // logger()<<"read: "<<buffer<<std::endl;
            std::string_view now_sim(buffer_in.data() + i * num_byte, num_byte);
            logger()<<"read: "<<now_sim;

            std::deque<std::string>::iterator iter = std::find(v_alph.begin(), v_alph.end(), now_sim);

            if (iter == v_alph.end()) {
                v_alph_const.emplace_back(now_sim);
                v_alph.emplace(v_alph.begin(), now_sim);
                buffer_out<<now_sim;
                logger()<<" write new: "<<now_sim<<std::endl;
            } else {
                buffer_out<<v_alph_const[iter - v_alph.begin()];
                logger()<<" write: "<<v_alph_const[iter - v_alph.begin()]<<std::endl;
                if (iter != v_alph.begin()) {
                    v_alph.erase(iter);
                    v_alph.emplace(v_alph.begin(), now_sim);
                }
            }
        }
        stream_out<<buffer_out.rdbuf();
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
        logger()<<"read str: "<<buffer<<" size = "<<read_bytes<<std::endl;
        if (read_bytes == 0) break;
        for (int i = 0; i < read_bytes / num_byte; i++) {
            // logger()<<"read: "<<buffer<<std::endl;
            std::string_view now_sim(buffer.data() + i * num_byte, num_byte);
            logger()<<"read: "<<now_sim;
            std::vector<std::string>::iterator iter = std::find(v_alph_const.begin(), v_alph_const.end(), now_sim);
            if (iter == v_alph_const.end()) {
                v_alph_const.emplace_back(now_sim);
                v_alph.emplace(v_alph.begin(), now_sim);
                stream_out<<now_sim;
                logger()<<"write new: "<<now_sim<<std::endl;
            } else {
                stream_out<<v_alph[iter - v_alph_const.begin()];
                logger()<<"write: "<<v_alph[iter - v_alph_const.begin()]<<std::endl;
                if (iter != v_alph_const.begin()) {
                    v_alph_const.erase(iter);
                    v_alph.emplace(v_alph.begin(), now_sim);
                }
            }
        }
    }
}
#undef BUFFER_SIZE
