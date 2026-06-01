#include "lexer.h"

std::istream& operator>>(std::istream& is, lexer& lx) {
    std::string str;
    std::string cur_str;
    while (std::getline(is, str)) {
        lexer::n_line++;
        str+=' ';
        for (size_t i = 0; i < str.size(); i++) {
            if (str[i] == '#') break;
            if (str[i] == '"') {
                if (cur_str.size()) {
                    std::cerr<<"ERROR! Can't parce "<<cur_str<<'.'<<std::endl;
                    cur_str.clear();
                }
                size_t end_str = str.find('"', i+1);
                if (end_str == str.npos) {
                    std::cerr<<"["<<lexer::n_line<<':'<<i<<"] "<<"ERROR! Can't parce string. lost \"."<<std::endl;
                    break;
                }
                cur_str+=str.substr(i+1, end_str - i - 1);
                i = end_str;
                lx.add_token({toke_type::string, cur_str, i, lexer::n_line});
                cur_str.clear();
                continue;
            }
            if (str[i] == ' ') {
                if (cur_str.size()) {
                    if (std::find(lx.commands.begin(), lx.commands.end(), cur_str) != lx.commands.end())
                        lx.add_token({toke_type::keyword, cur_str, i, lexer::n_line});
                    else try {
                        size_t idx;
                        std::stof(cur_str, &idx);
                        if (idx == cur_str.size()) {
                            lx.add_token({toke_type::number, cur_str, i, lexer::n_line});
                        } else throw ;
                    } catch(...){
                        lx.add_token({toke_type::unknown, cur_str, i, lexer::n_line});
                        std::cerr<<"["<<lexer::n_line<<':'<<i<<"] "<<"ERROR! Unknown word: "<<cur_str<<'.'<<std::endl;
                    }
                    cur_str.clear();
                }
                continue;
            } if (str[i] == '=') {
                lx.add_token({toke_type::identifier, cur_str, i, lexer::n_line});
                cur_str.clear();
                continue;
            }
            cur_str += str[i];
        }
    }
    return is;
}
void lexer::add_command(const std::string &command) {
    commands.push_back(command);
}

std::vector<token> lexer::get_tokens() {
    return v_tokens;
}
void lexer::clear() {
    v_tokens.clear();
}

void lexer::add_token(const token &tkn) {
    v_tokens.push_back(tkn);
}
