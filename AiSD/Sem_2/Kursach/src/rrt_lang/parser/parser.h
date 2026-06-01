#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

#include "../lexer/lexer.h"
#include "../../space/world.h"

class error_parse : std::exception {
    const std::string message;
public:
    error_parse(const std::string& message);
    virtual const char *what() const throw() override;
};

class command {
protected:
    const std::vector<std::string> keywords;
    const token* tkn = nullptr;
public:
    command(const std::vector<std::string> &keywords);
    virtual void get(const std::vector<token> &tokens, size_t &iter) = 0;
    virtual void do_work(world& wrld) = 0;
    virtual std::unique_ptr<command> clone() const = 0;
    void set_token(const token *tkn);
    std::string gen_err(const std::string& message);
    virtual ~command() = default;
};

class parser {
    inline static std::unordered_map<std::string_view, command*> c_trans;
public:
  static void add_command(const std::string& keyword, command* command);
  static std::vector<std::unique_ptr<command>> parce(const std::vector<token> &tokens);
};
