#include "parser.h"

error_parse::error_parse(const std::string& message) : message(message) {}

const char *error_parse::what() const throw() {
    return message.data();
}

command::command(const std::vector<std::string> &keywords) : keywords(keywords) {
    for (const std::string& keyword : this->keywords) {
        lexer::add_command(keyword);
        parser::add_command(keyword, this);
    }
}

void command::set_token(const token *tkn) {
    this->tkn=tkn;
}

std::string command::gen_err(const std::string& message) {
    std::string str;
    if (tkn) {
        str+="Parsing error. [" + std::to_string(tkn->line) + ":" + std::to_string(tkn->pos) + "]: " + tkn->str + ". ";
    }
    str+=message;
    return str;
}

void parser::add_command(const std::string& keyword, command *command) {
    c_trans[keyword] = command;
}

std::vector<std::unique_ptr<command>> parser::parce(const std::vector<token> &tokens) {
    std::vector<std::unique_ptr<command>> v_commands;
    std::unordered_map<std::string_view, command*>::iterator it;
    for (size_t i = 0; i < tokens.size(); i++) {
        switch (tokens[i].type) {
            case toke_type::keyword:
                it = c_trans.find(tokens[i].str);
                    if (it != c_trans.end()) {
                        v_commands.emplace_back(it->second->clone());
                        v_commands.back()->set_token(&tokens[i]);
                        try {
                            v_commands.back()->get(tokens, i);
                        } catch(error_parse& er) {
                            std::cerr<<er.what()<<std::endl;
                            v_commands.pop_back();
                        }
                    }
                break;

            default:
                break;
        }
    }
    return v_commands;
}
