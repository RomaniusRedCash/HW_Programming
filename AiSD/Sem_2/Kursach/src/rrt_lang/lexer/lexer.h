#pragma once

#include <string>
#include <vector>
#include <iostream>


enum class toke_type {
    keyword,
    identifier,
    string,
    number,
    assign,
    unknown
};

struct token {
    toke_type type;
    std::string str;
    size_t pos = 0, line = 0;
};

class lexer {
    inline static std::vector<std::string_view> commands;
    std::vector<token> v_tokens;
public:
    inline static size_t n_line = 0;
    static void add_command(const std::string &command);
    std::vector<token> get_tokens();
    void clear();
    void add_token(const token &tkn);
    friend std::istream& operator>>(std::istream &is, lexer &lx);
};
