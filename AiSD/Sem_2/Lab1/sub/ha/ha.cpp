#include "ha.h"
#include <memory>
#include <utility>

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
    if (!(size++%8)){
        data.push_back(0x80);
    }else {
        // data.back();
        data.back() |= 1 << (8 - size % 8);
    }
    // size++;
}

void ha_code::pop_back() {
    size--;
    if (!(size%8)) data.pop_back();
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

calculator::calculator(const std::vector<node*>& v_node_in) : v_node(v_node_in){
    if (v_node.size() < 2) throw"meni duje lenivo";
    while (v_node.size() > 1) {
        std::vector<node*>::iterator it1 = v_node.begin(), it2 = v_node.begin();
        for (std::vector<node*>::iterator i = v_node.begin(); i != v_node.end(); i++)
            it1 = **it1 < **i ? it1 : i;
        it2 = it1 == v_node.begin() ? it2 + 1 : it2;
        for (std::vector<node*>::iterator j = v_node.begin(); j != v_node.end(); j++)
            if (it1 != j) it2 = **it2 < **j ? it2 : j;
        if (it1 > it2) std::swap(it1, it2);
        logger(log_ns::DEV_ONLY) << (*it1)->data<<" : "<<(*it1)->num << " and ";
        logger(log_ns::DEV_ONLY) << (*it2)->data<<" : "<<(*it1)->num <<std::endl;

#ifdef DEBUG
        node* nn = new node((*it1)->data + (*it2)->data,(*it1)->num + (*it2)->num);
#else
        node* nn = new node((*it1)->num + (*it2)->num);
#endif
        nn->left = *it1;
        nn->right = *it2;
        // if (it1 < it2) {
            *it2 = nn;
            v_node.erase(it1);
        // } else {
            // *it1 = nn;
            // v_node.erase(it2);
        // }
        // v_node.push_back(nn);
        logger(log_ns::DEV_ONLY) << "pool: ";
        for (const node* i :v_node)
            logger(log_ns::DEV_ONLY)<<i->data << ' ';
        logger(log_ns::DEV_ONLY)<<std::endl;
    }
    logger(log_ns::DEV_ONLY)<<"end treeing"<<std::endl;
    ha_code hc;
    order(v_node.front(), hc, 0);
    logger(log_ns::DEV_ONLY)<<"end ordering"<<std::endl;

    std::queue<node*> q_nodes;
    q_nodes.push(v_node.front());
    while (!q_nodes.empty()) {
        node* current = q_nodes.front();
        q_nodes.pop();
        if (current) {
            if (current->left) q_nodes.push(current->left);
            if (current->right) q_nodes.push(current->right);
            delete current;
        }
    }
    logger(log_ns::DEV_ONLY)<<"end clearing"<<std::endl;
}

void calculator::order(node*& noda, ha_code& hc, const int8_t& balance) {
    if (noda) {
        if (balance == -1) hc.add_null();
        else if (balance == 1) hc.add_one();

        logger(log_ns::DEV_ONLY) << noda->data << " : ";
        for (const uint8_t& c : hc.get_data())
            logger(log_ns::DEV_ONLY)<<std::bitset<8>(c);
        logger(log_ns::DEV_ONLY)<<" size "<<hc.get_size()<<std::endl;

        if (!noda->left && !noda->right) {
            mapa[noda->data]=hc;
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

const ha_code& calculator::operator[] (const std::string& sim) const {
    return mapa.at(sim);
}

const std::unordered_map<std::string, ha_code>::const_iterator calculator::begin() const {
    return mapa.cbegin();
}
const std::unordered_map<std::string, ha_code>::const_iterator calculator::end() const {
    return mapa.cend();
}

//////////////////////////////////// fun ////////////////////////////////////

#define BUFFER_SIZE 1024 * num_byte
void ha_ns::ha_1(std::istream& stream_in, std::ostream& stream_out, const int& num_byte, const calculator& calc) {
    std::string buffer(BUFFER_SIZE, 0);
    size_t read_bites = 0;
    sstrtobb bbs_out;
    for (const std::pair<std::string, ha_code>& i :calc) {
        bbs_out<<i.first;
        // const char* cs = std::static_cast<const char*>(&i.second.get_size());

        bbs_out.write(, );
    }
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
}

void ha(std::istream& stream_in, std::ostream& stream_out, const int& num_byte) {
    std::map<std::string, size_t> mapa;
    std::string buffer(BUFFER_SIZE, 0);
    size_t read_bites = 0;
    while(stream_in) {
        stream_in.read(buffer.data(), BUFFER_SIZE);
        read_bites = stream_in.gcount();
        logger(log_ns::DEV_ONLY)<<"read ";
        logger(log_ns::DEV_ONLY).write(buffer.data(), read_bites);
        logger(log_ns::DEV_ONLY)<<" size "<<read_bites<<std::endl;
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
    // for (node* i : v_nodes) delete i;
    // logger(log_ns::DEV_ONLY) << "final clearing"<<std::endl;
    logger()<<"model: "<<std::endl;
    for (const std::pair<std::string, ha_code>& i : calc) {
        logger()<<'\t'<<i.first<<" : ";
        for (const uint8_t& c : i.second.get_data())
            logger()<<std::bitset<8>(c);
        logger()<<" size " << i.second.get_size()<<std::endl;
    }
    stream_in.clear();
    stream_in.seekg(0, std::ios::beg);
    ha_ns::ha_1(stream_in,stream_out,num_byte,calc);
}
#undef BUFFER_SIZE


































