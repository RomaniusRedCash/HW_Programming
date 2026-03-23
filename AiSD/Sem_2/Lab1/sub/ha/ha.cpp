#include "ha.h"
#include "ha_sub.h"
#include <cstdint>
#include <cstdlib>
#include <string>

using namespace ha_ns;

//////////////////////////////////// ha_code ////////////////////////////////////

ha_code::ha_code() : bytebit(0){};

void ha_code::add_null() {
    if (!(size%8)){
        data.push_back(0);
    }else {
        data.back() &= 0xFF << (8 - size % 8);
    }
    size++;
}
void ha_code::add_one() {
    if (!(size%8)){
        data.push_back(0x80);
    }else {
        // data.back();
        data.back() |= 0x80 >> size%8;
    }
    size++;
}

void ha_code::pop_back() {
    size--;
    data.back() &= 0xFF << (8-size%8);
    if (!(size%8)) data.pop_back();
}

bool ha_code::read_hacode(sstrtobb& ssbb, const calculator& calc, const size_t& max_size) {
    while (ssbb.get_data().size() * 8 - 8 + ssbb.get_buffer_sdvig_size() > size) {
        logger(log_ns::DEV_ONLY | log_ns::HARD_LVL)<<std::bitset<8>(ssbb.get_data()[size/8])<<"&"<<std::bitset<8>(0x80 >> size % 8)<<" pos " << bool(ssbb.get_data()[size/8] & (0x80 >> size % 8))  << std::endl;

        if (ssbb.get_data()[size/8] & (0x80 >> size % 8)) {
            add_one();
        }
        else add_null();
        if (calc.have_hac(*this)) {
            logger(log_ns::NORMAL_LVL)<<"succes read ";
            for (const char& c : data)
                logger(log_ns::NORMAL_LVL) << std::bitset<8>(c);
            logger(log_ns::NORMAL_LVL)<<" size " << size <<" : "<<calc[*this]<<std::endl;
            ssbb.sdvig(-get_size());
            return true;
        }
        for (const char& c : data)
            logger(log_ns::NORMAL_LVL) << std::bitset<8>(c);
        logger(log_ns::NORMAL_LVL)<<" size "<<size<<" not found"<<std::endl;
        if (get_size() >= max_size) {
            logger(log_ns::DEV_ONLY) << "already max size " <<max_size<<std::endl;
            return false;
        }
    }
    return false;
}

bool ha_code::operator<(const ha_code& hc) const {
    if (size != hc.size) return size < hc.size;
    for (size_t i = 0; i < size / 8; i++)
        if (data[i] < hc.data[i]) return true;
    return (data.back() >> (8-size%8)) < (hc.data.back() >> (8-size%8));
}

//////////////////////////////////// node ////////////////////////////////////

node::node(const size_t& num) : num(num) {
}
node::node(const std::string& data, const size_t& num) : data(data), num(num) {
}

bool node::operator<(const node& noda) {
    return num < noda.num;
}

//////////////////////////////////// calculator ////////////////////////////////////

void calculator::order(node*& noda, ha_code& hc, const int8_t& balance) {
    if (noda) {
        if (balance == -1) hc.add_null();
        else if (balance == 1) hc.add_one();

        logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL) << noda->data << " : ";
        for (const uint8_t& c : hc.get_data())
            logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<std::bitset<8>(c);
        logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<" size "<<hc.get_size()<<std::endl;

        if (!noda->left && !noda->right) {
            zap(noda->data, hc);
            v_data.push_back(noda->data);
            logger(log_ns::DEV_ONLY)<<"leaf: "<<noda->data<<" : ";
            for (const uint8_t& c : hc.get_data())
                logger(log_ns::DEV_ONLY)<<std::bitset<8>(c);
            logger(log_ns::DEV_ONLY)<<" size "<<hc.get_size()<<std::endl;
        } else {
            order(noda->left, hc, -1);
            order(noda->right, hc, 1);
        }
        if (hc.get_size())
            hc.pop_back();

    }
}

void calculator::zap(const std::string& sim, const ha_code& ha_c) {
    if (read)
        de_mapa[ha_c]=sim;
    else mapa[sim]=ha_c;
}

void calculator::clear(node* n) {
    std::queue<node*> q_nodes;
    q_nodes.push(n);
    while (!q_nodes.empty()) {
        node* current = q_nodes.front();
        q_nodes.pop();
        if (current) {
            if (current->left) q_nodes.push(current->left);
            if (current->right) q_nodes.push(current->right);
            delete current;
        }
    }
    v_node.clear();
    logger(log_ns::DEV_ONLY)<<"end clearing"<<std::endl;
}

void calculator::remap(const std::map<size_t, std::vector<std::string>>& mapa_in) {
    v_data.clear();
    logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<"max lvl "<<std::prev(mapa_in.end())->first<<std::endl;
    std::vector<std::vector<node*>> v_tmp(std::prev(mapa_in.end())->first + 1);
    for (long long i = v_tmp.size() - 1; i >= 0; i--) {
        logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<"lvl is "<<i<<std::endl;
        if (i + 1 < v_tmp.size())
            for (size_t j = 0; j < v_tmp[i + 1].size() - 1; j+= 2) {
                node* nn = new node(v_tmp[i+1][j]->data + v_tmp[i+1][j + 1]->data, 0);
                nn->left = v_tmp[i+1][j];
                nn->right = v_tmp[i+1][j + 1];
                v_tmp[i].push_back(nn);
                logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL) << "add "<<nn->data<<" from lvl "<<i+1 << std::endl;
            }
        if (mapa_in.find(i) != mapa_in.end())
            for (const std::string& j : mapa_in.at(i)) {
                v_tmp[i].push_back(new node(j, 0));
                logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL) << "add "<<j<<" from lvl "<<i << std::endl;
            }
    }
    v_node.push_back(v_tmp.front().front());
}

calculator::calculator(const std::vector<node*>& v_node_in) : v_node(v_node_in){
    if (v_node.size() < 2) throw"meni duje lenivo";
    // std::sort(v_node.begin(), v_node.end(), [](node* a, node* b) { return *b < *a; });
    while (v_node.size() > 1) {
        std::vector<node*>::iterator it1 = v_node.begin(), it2 = v_node.begin() + 1;
        if (**it2 < **it1) std::swap(it1, it2);
        for (std::vector<node*>::iterator i = v_node.begin() + 2; i != v_node.end(); i++)
            if (**i < **it2) {
                it1 = **it2 < **it1 ? it2 : it1;
                it2 = i;
            }

        // if (it1 > it2) std::swap(it1, it2);
        // if((*it1)->depth < (*it2)->depth)
            // std::swap(it1, it2);

#ifdef DEBUG
        node* nn = new node((*it1)->data + (*it2)->data,(*it1)->num + (*it2)->num);
#else
        node* nn = new node((*it1)->num + (*it2)->num);
#endif
        nn->depth = (*it1)->depth + 1;
        nn->left = *it1;
        nn->right = *it2;

        logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL) << (*it1)->data<<" : "<<(*it1)->num << " and ";
        logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL) << (*it2)->data<<" : "<<(*it2)->num <<std::endl;

        if (it1 > it2) std::swap(it1, it2);
        *it2 = nn;
        // v_node.erase(it2);
        v_node.erase(it1);
        // v_node.push_back(nn);

        logger(log_ns::DEV_ONLY | log_ns::HARD_LVL) << "pool: ";
        for (const node* i :v_node)
            logger(log_ns::DEV_ONLY | log_ns::HARD_LVL)<<i->data << ' ';
        logger(log_ns::DEV_ONLY | log_ns::HARD_LVL)<<std::endl;
    }
    ha_code hc;
    order(v_node.front(), hc, 0);
    clear(v_node.front());

    std::map<size_t, std::vector<std::string>> mapa2;
    for (const std::pair<std::string, ha_code>& i : mapa)
        mapa2[i.second.get_size()].push_back(i.first);
    remap(mapa2);

    logger(log_ns::DEV_ONLY)<<"end treeing"<<std::endl;
    order(v_node.front(), hc, 0);
    logger(log_ns::DEV_ONLY)<<"end ordering"<<std::endl;

    clear(v_node.front());
    logger()<<print_model();
}

calculator::calculator(const std::map<size_t, std::vector<std::string>>& mapa_in) {
    read = true;

    remap(mapa_in);

    logger(log_ns::DEV_ONLY)<<"end treeing"<<std::endl;
    ha_code hc;
    order(v_node.front(), hc, 0);
    logger(log_ns::DEV_ONLY)<<"end ordering"<<std::endl;

    clear(v_node.front());
    logger()<<print_model();
}

const ha_code& calculator::operator[] (const std::string& sim) const {
    return mapa.at(sim);
}

const std::string& calculator::operator[] (const ha_code& ha_c) const {
    return de_mapa.at(ha_c);
}

bool calculator::have_hac (const ha_code& ha_c) const {
    return de_mapa.find(ha_c) != de_mapa.end();
}

const std::map<std::string, ha_code>::const_iterator calculator::begin() const {
    return mapa.cbegin();
}
const std::map<std::string, ha_code>::const_iterator calculator::end() const {
    return mapa.cend();
}

const size_t calculator::get_size() const {
    return mapa.size();
}

const std::map<std::string, ha_code>& calculator::get_mapa() const {
    return mapa;
}

std::string calculator::print_model() {
    std::stringstream os;
    os<<"model: "<<std::endl;
    if (read) for (const std::pair<ha_code, std::string>& i : de_mapa) {
        os<<'\t'<<i.second<<" : ";
        for (const uint8_t& c : i.first.get_data())
            os<<std::bitset<8>(c);
        os<<" size " << i.first.get_size()<<std::endl;
    }
    else for (const std::string& str : v_data) {
        os<<'\t'<<str<<" : ";
        for (const uint8_t& c : mapa[str].get_data())
            os<<std::bitset<8>(c);
        os<<" size " << mapa[str].get_size()<<std::endl;
    }
    return os.str();
}

const std::vector<std::string>& calculator::get_ord() const {
    return v_data;
}

//////////////////////////////////// fun ////////////////////////////////////
/*
 * last byte size
 * byte of model size
 * model size
 * byte of node size
 * node size
*/



#define BUFFER_SIZE 1024 * num_byte
void ha_ns::ha_1(std::istream& stream_in, std::ostream& stream_out, const uint8_t& num_byte, const calculator& calc) {
// last byte
    stream_out.put(0);

// model size
    ha_ns::write_shift_size(stream_out, calc.get_size());
    write_shift_size(stream_out, calc);

    sstrtobb bbs_out;
    std::string buffer(BUFFER_SIZE, 0);
    size_t read_bites = 0;

    while(stream_in) {
        stream_in.read(buffer.data(), BUFFER_SIZE);
        read_bites = stream_in.gcount();
        // sstrtobb bbs_in(buffer);
        for (size_t i = 0; i < read_bites / num_byte; i++) {
            // bytebit bb(num_byte);
            // bbs_in>>bb;
            bbs_out<<calc[buffer.substr(i * num_byte, num_byte)];
            bbs_out.try_write(stream_out);
        }
        if (read_bites < BUFFER_SIZE) break;
    }
    if (size_t ost = read_bites % num_byte)
        bbs_out<<calc[buffer.substr(read_bites - ost, ost)];
    stream_out << bbs_out.get_data();
    stream_out.clear();
    stream_out.seekp(0, std::ios::beg);
    stream_out << bbs_out.real_last();
}

void ha(std::istream& stream_in, std::ostream& stream_out, const uint8_t& num_byte) {
    std::map<std::string, size_t> mapa;
    std::string buffer(BUFFER_SIZE, 0);
    size_t read_bites = 0;
    while(stream_in) {
        stream_in.read(buffer.data(), BUFFER_SIZE);
        read_bites = stream_in.gcount();
        logger(log_ns::DEV_ONLY | log_ns::HARD_LVL)<<"read ";
        logger(log_ns::DEV_ONLY | log_ns::HARD_LVL).write(buffer.data(), read_bites);
        logger(log_ns::DEV_ONLY | log_ns::HARD_LVL)<<" size "<<read_bites<<std::endl;
        for (size_t i = 0; i < read_bites / num_byte; i++) {
            mapa[buffer.substr(i * num_byte, num_byte)]++;
        }
        if (read_bites < BUFFER_SIZE) break;
    }
    if (size_t ost = read_bites % num_byte)
        mapa[buffer.substr(read_bites - ost, ost)]++;
    std::vector<node*> v_nodes;
    logger()<<"nabor: "<<std::endl;
    for (const std::pair<std::string, size_t> i : mapa) {
        node* nn = new node(i.first, i.second);
        v_nodes.push_back(nn);
        logger()<<i.first<<" : " <<i.second<<std::endl;
    }
    calculator calc(v_nodes);
    stream_in.clear();
    stream_in.seekg(0, std::ios::beg);
    ha_ns::ha_1(stream_in,stream_out,num_byte,calc);
}

void de_ha(std::istream& stream_in, std::ostream& stream_out, const uint8_t& num_byte) {
    uint8_t last_byte_size = 0;
    size_t model_size = 0;
    stream_in >> last_byte_size;
    ha_ns::read_shift_size(stream_in, model_size);
    logger(log_ns::DEV_ONLY) <<"last byte len: "<<size_t(last_byte_size) << " model size: "<<model_size<<std::endl;
    std::map<size_t, std::vector<std::string>> mapa;
    uint8_t node_size;
    stream_in >> node_size;
    ha_ns::read_shift_size(stream_in, mapa, model_size, num_byte, node_size);
    size_t max_size_sim = std::prev(mapa.end())->first;
    calculator calc(mapa);

    std::string buffer(BUFFER_SIZE, 0);
    sstrtobb ssbb;
    while(!stream_in.eof()) {
        stream_in.read(buffer.data(), BUFFER_SIZE);
        size_t read_bites = stream_in.gcount();

        char c;
        if (stream_in.get(c))
            stream_in.seekg(-1, std::ios::cur);
        else buffer.resize(read_bites);

        logger(log_ns::DEV_ONLY | log_ns::HARD_LVL)<<"read ";
        logger(log_ns::DEV_ONLY | log_ns::HARD_LVL).write(buffer.data(), read_bites);
        logger(log_ns::DEV_ONLY | log_ns::HARD_LVL)<<" size "<<read_bites<<std::endl;

#ifdef DEBUG
        logger(log_ns::DEV_ONLY)<<"new buffer is ";
        for (const char& c : buffer)
            logger(log_ns::DEV_ONLY)<<std::bitset<8>(c);
        logger(log_ns::DEV_ONLY)<<std::endl;
#endif


        if (stream_in.eof()){
            sstrtobb ssbb_tmp(buffer);
            ssbb_tmp.set_buffer_sdvig_size(last_byte_size);
            ssbb << ssbb_tmp;
        } else ssbb<<buffer;
            // ssbb.set_buffer_sdvig_size((ssbb.get_buffer_sdvig_size() + last_byte_size - 8) % 8);
        while (ssbb.get_data().size()) {
            ha_code hc;
            if (!hc.read_hacode(ssbb, calc, max_size_sim)) {
                logger(log_ns::DEV_ONLY | log_ns::NORMAL_LVL)<<"next buffer"<<std::endl;
                break;
            }
            stream_out<<calc[hc];
        }
    }
}
#undef BUFFER_SIZE
































