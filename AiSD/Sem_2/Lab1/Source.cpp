#include "Header.h"

std::queue<std::string> sub_commands;
bimap<std::string, const some_param*> map_translater;
uint8_t num_byte = 1;

void print_help() {
    for (const some_param& i : v_someprm){
        std::cout<<"\t--"<<std::left<<std::setw(20)<<i.name;

        size_t dvoet = i.desc.find_first_of(':');
        std::cout<<std::left<<std::setw(10)<<':';
        std::cout<< std::string(i.desc.begin() + dvoet + 1, i.desc.end()) <<std::endl;
    }
}

int main(const int argc, char* argv[]) {
#ifndef NDEBUG
    logger_demon::add_log_lvl(log_ns::DEV_ONLY);
#endif
    std::vector<option> long_opt;
    for (const some_param& i : v_someprm) {
        map_translater.insert(i.name, &i);
        long_opt.push_back(i.opt);
    }

    int opt, long_idx;
    std::string str_inp, str_out, str_cmpr;
    std::string str_itu;
    std::vector<const some_param*> v_params;

    while ((opt = getopt_long(argc, argv, "hi:o:", &long_opt.front(), &long_idx)) != -1)
        switch (opt) {
            case 'h':
                print_help();
                return 0;
                break;
            case 'i':
                str_inp = optarg;
                break;
            case 'o':
                str_out = optarg;
                break;
            case 0:
                if (map_translater[long_opt[long_idx].name]->add_to_list) {
                    v_params.push_back(map_translater[long_opt[long_idx].name]);
                    sub_commands.push(optarg != nullptr ? optarg : "");
                }
                switch (const eCOMMANDS ec = map_translater[long_opt[long_idx].name]->ec) {
                    case eBYTE:
                        num_byte = std::stoi(optarg);
                        if (num_byte == 0) {
                            std::cout<<"ochen smeshno"<<std::endl;
                            return 0;
                        }
                        break;
                    case eLOGF:
                        logger_demon::open_logfile("log.log");
                        logger_demon::add_log_lvl(log_ns::FILE_LVL);
                        break;
                    case eLOGC:
                        logger_demon::add_log_lvl(log_ns::CONS_LVL);
                        break;
                    case eLOGH:
                        logger_demon::add_log_lvl(log_ns::HARD_LVL);
                    case eLOGN:
                        logger_demon::add_log_lvl(log_ns::NORMAL_LVL);
                        break;

                    default:
                        break;
                }
                break;
            default:
                break;
        }

    if (str_inp.empty()) logger()<<"No input path."<<std::endl;
    if (str_out.empty()) logger()<<"No output path. It will continue without output file."<<std::endl;
    std::fstream file_in(str_inp, std::ios::in | std::ios::binary);
    std::stringstream ss_tmp1,ss_tmp2;
    std::stringstream *p_ss_tmp1 = &ss_tmp1, *p_ss_tmp2 = &ss_tmp2;
    std::fstream file_out;
    if (file_in.is_open())
        ss_tmp1 << file_in.rdbuf();
    for (const some_param*& prm : v_params) {
        p_ss_tmp2->str(std::string());
        p_ss_tmp2->clear();
        p_ss_tmp2->seekp(0,std::ios::beg);
        p_ss_tmp1->clear();
        p_ss_tmp1->seekg(0,std::ios::beg);
        switch (prm->ec) {
            case eENWIKn7:
                start_algorithm(prm->name, [&]{enwik8_to_enwik(*p_ss_tmp1, *p_ss_tmp2);});
                break;

            // NOTE: compress
            case eRLE:
                start_algorithm(prm->name, [&]{RLE2(*p_ss_tmp1, *p_ss_tmp2, num_byte);});
                break;
            case eMTF:
                start_algorithm(prm->name, [&]{mtf(*p_ss_tmp1, *p_ss_tmp2, num_byte);});
                break;
            case eHA:
                start_algorithm(prm->name, [&]{ha(*p_ss_tmp1, *p_ss_tmp2, num_byte);});
                break;
            case eBWT:
                start_algorithm(prm->name, [&]{bwt(*p_ss_tmp1, *p_ss_tmp2);});
                break;
            case eLZW:
                start_algorithm(prm->name, [&]{lzw(*p_ss_tmp1, *p_ss_tmp2);});
                break;
            case eLZSS:
                start_algorithm(prm->name, [&]{lzss(*p_ss_tmp1, *p_ss_tmp2);});
                break;

            // NOTE: decompress
            case eDERLE:
                start_algorithm(prm->name, [&]{from_RLE2(*p_ss_tmp1, *p_ss_tmp2, num_byte);});
                break;
            case eDEMTF:
                start_algorithm(prm->name, [&]{de_mtf(*p_ss_tmp1, *p_ss_tmp2, num_byte);});
                break;
            case eDEHA:
                start_algorithm(prm->name, [&]{de_ha(*p_ss_tmp1, *p_ss_tmp2, num_byte);});
                break;
            case eDEBWT:
                start_algorithm(prm->name, [&]{de_bwt(*p_ss_tmp1, *p_ss_tmp2);});
                break;
            case eDELZW:
                start_algorithm(prm->name, [&]{de_lzw(*p_ss_tmp1, *p_ss_tmp2);});
                break;
            case eDELZSS:
                start_algorithm(prm->name, [&]{de_lzss(*p_ss_tmp1, *p_ss_tmp2);});
                break;


            // NOTE: JPEG
            case eITU:
                start_algorithm(prm->name, [&]{itu(*p_ss_tmp1, *p_ss_tmp2, itu_ns::get_layer(str_itu));});
                break;
            case eDEITU:
                start_algorithm(prm->name, [&]{de_itu(*p_ss_tmp1, *p_ss_tmp2, itu_ns::get_layer(str_itu));});
                break;

#ifndef NDEBUG
            case eTEST:
                test("abacabacabadaca", num_byte);
                break;
#endif
            default:
                break;
        }
        sub_commands.pop();
        std::swap(p_ss_tmp1, p_ss_tmp2);
    }
    file_out.open(str_out, std::ios::out | std::ios::binary);
    if (!file_out.is_open()) {
        logger() << "ERROR! Can't create out file."<<std::endl;
        return 1;
    } else {
        p_ss_tmp1->clear();
        p_ss_tmp1->seekg(0,std::ios::beg);
        file_out<<p_ss_tmp1->rdbuf();
    }
    file_out.close();
    file_in.close();

    return 0;
}
