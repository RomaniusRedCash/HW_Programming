#include "entrop.h"
#include <cmath>

double calc_entrop (std::string str, const uint8_t& len_sim) {
    if (str.size()%len_sim) throw "Len ERROR";
    std::map<std::string, int> ver;
    int num_sim = str.size()/len_sim;
    for (int i = 0; i < num_sim; i++) {
        ver[{str.begin() + i * len_sim, str.begin() + (i + 1) * len_sim}]++;
    }
    double H = 0;
    for (const std::pair<std::string, int>& i : ver) {
        double P = i.second; P /= num_sim;
        H-=P*std::log2(P);
    }
    return H;
}
