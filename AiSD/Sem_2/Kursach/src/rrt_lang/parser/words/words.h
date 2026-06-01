#pragma once

#include "../parser.h"
#include "../../../object/rectangle/rectangle.h"

#define CONCAT_HIDDEN(a, b) a ## b
// #define CONCAT_HIDDEN3(a, b, c) a ## b ## c
#define CONCAT(a, b) CONCAT_HIDDEN(a, b)

#define NAME CONCAT(box, _word)
class NAME : public command {
    float x,y,w,h;
    bool fill = true;
public:
    NAME() : command({"box", "b"}) {}
    void get(const std::vector<token> &tokens, size_t &iter) override {
        if (tokens[iter+1].type != toke_type::number ||
            tokens[iter+2].type != toke_type::number ||
            tokens[iter+3].type != toke_type::number ||
            tokens[iter+4].type != toke_type::number
        ) throw error_parse(gen_err("Failed getting box."));
        x = std::stof(tokens[iter+1].str);
        y = std::stof(tokens[iter+2].str);
        w = std::stof(tokens[iter+3].str);
        h = std::stof(tokens[iter+4].str);
        iter+=4;
    }
    void do_work(world& wrld) override {
        if(wrld.block_map) {
            std::cout<<gen_err("Failed getting box.")<<std::endl;
            return;
        }
        std::cout<<"Create box with id: "<< wrld.add_object(rectangle(x,y,w,h))<<std::endl;
    }
    std::unique_ptr<command> clone() const override {
        return std::make_unique<NAME>(*this);
    }
} CONCAT(base,NAME);
#undef NAME
#define NAME CONCAT(map, _word)
class NAME : public command {
    float w,h;
    bool fill = true;
public:
    NAME() : command({"map"}) {}
    void get(const std::vector<token> &tokens, size_t &iter) override {
        if (tokens[iter+1].type != toke_type::number ||
            tokens[iter+2].type != toke_type::number
        ) throw error_parse(gen_err("Failed getting size map."));
        w = std::stof(tokens[iter+1].str);
        h = std::stof(tokens[iter+2].str);
        iter+=4;
    }
    void do_work(world& wrld) override {
        wrld.resize(w,h);
    }
    std::unique_ptr<command> clone() const override {
        return std::make_unique<NAME>(*this);
    }
} CONCAT(base,NAME);
#undef NAME
#define NAME CONCAT(set_start, _word)
class NAME : public command {
    float x,y;
public:
    NAME() : command({"start"}) {}
    void get(const std::vector<token> &tokens, size_t &iter) override {
        if (tokens[iter+1].type != toke_type::number ||
            tokens[iter+2].type != toke_type::number
        ) throw error_parse(gen_err("Failed getting pos."));
        x = std::stof(tokens[iter+1].str);
        y = std::stof(tokens[iter+2].str);
        iter+=4;
    }
    void do_work(world& wrld) override {
        wrld.rth->set_start(x, y);
    }
    std::unique_ptr<command> clone() const override {
        return std::make_unique<NAME>(*this);
    }
} CONCAT(base,NAME);
#undef NAME
#define NAME CONCAT(set_end, _word)
class NAME : public command {
    float x,y;
public:
    NAME() : command({"end"}) {}
    void get(const std::vector<token> &tokens, size_t &iter) override {
        if (tokens[iter+1].type != toke_type::number ||
            tokens[iter+2].type != toke_type::number
        ) throw error_parse(gen_err("Failed getting pos."));
        x = std::stof(tokens[iter+1].str);
        y = std::stof(tokens[iter+2].str);
        iter+=4;
    }
    void do_work(world& wrld) override {
        wrld.rth->set_end(x, y);
    }
    std::unique_ptr<command> clone() const override {
        return std::make_unique<NAME>(*this);
    }
} CONCAT(base,NAME);
#undef NAME
#define NAME CONCAT(clear, _word)
class NAME : public command {
public:
    NAME() : command({"clear"}) {}
    void get(const std::vector<token> &tokens, size_t &iter) override {}
    void do_work(world& wrld) override {
        wrld.clear();
    }
    std::unique_ptr<command> clone() const override {
        return std::make_unique<NAME>(*this);
    }
} CONCAT(base,NAME);
#undef NAME
#define NAME CONCAT(delete, _word)
class NAME : public command {
    size_t id;
public:
    NAME() : command({"del"}) {}
    void get(const std::vector<token> &tokens, size_t &iter) override {
        if (tokens[iter+1].type != toke_type::number)
            throw error_parse(gen_err("Failed getting id."));
        id = std::stof(tokens[iter+1].str);
        iter++;
    }
    void do_work(world& wrld) override {
        wrld.del(id);
    }
    std::unique_ptr<command> clone() const override {
        return std::make_unique<NAME>(*this);
    }
} CONCAT(base,NAME);
#undef NAME
#define NAME CONCAT(go, _word)
class NAME : public command {
    size_t iterations;
public:
    NAME() : command({"go"}) {}
    void get(const std::vector<token> &tokens, size_t &iter) override {
        if (tokens[iter+1].type != toke_type::number)
            throw error_parse(gen_err("Failed getting id."));
        iterations = std::stof(tokens[iter+1].str);
        iter++;
    }
    void do_work(world& wrld) override {
        wrld.rth->start_routing(iterations);
    }
    std::unique_ptr<command> clone() const override {
        return std::make_unique<NAME>(*this);
    }
} CONCAT(base,NAME);
#undef NAME
#define NAME CONCAT(rrt, _word)
class NAME : public command {
public:
    NAME() : command({"rrt"}) {}
    void get(const std::vector<token> &tokens, size_t &iter) override {
    }
    void do_work(world& wrld) override {
        wrld.turn(rrt_type::standart);
    }
    std::unique_ptr<command> clone() const override {
        return std::make_unique<NAME>(*this);
    }
} CONCAT(base,NAME);
#undef NAME
#define NAME CONCAT(rrtz, _word)
class NAME : public command {
public:
    NAME() : command({"rrt*"}) {}
    void get(const std::vector<token> &tokens, size_t &iter) override {
    }
    void do_work(world& wrld) override {
        wrld.turn(rrt_type::zvezda);
    }
    std::unique_ptr<command> clone() const override {
        return std::make_unique<NAME>(*this);
    }
} CONCAT(base,NAME);
#undef NAME
#define NAME CONCAT(fastrrt, _word)
class NAME : public command {
public:
    NAME() : command({"fastrrt"}) {}
    void get(const std::vector<token> &tokens, size_t &iter) override {
    }
    void do_work(world& wrld) override {
        wrld.turn(rrt_type::fast);
    }
    std::unique_ptr<command> clone() const override {
        return std::make_unique<NAME>(*this);
    }
} CONCAT(base,NAME);
#undef NAME
#define NAME CONCAT(setdist, _word)
class NAME : public command {
    float dist_way;
public:
    NAME() : command({"step"}) {}
    void get(const std::vector<token> &tokens, size_t &iter) override {
        if (tokens[iter+1].type != toke_type::number)
            throw error_parse(gen_err("Failed getting id."));
        dist_way = std::stof(tokens[iter+1].str);
        iter++;
    }
    void do_work(world& wrld) override {
        wrld.rth->set_dist_way(dist_way);
    }
    std::unique_ptr<command> clone() const override {
        return std::make_unique<NAME>(*this);
    }
} CONCAT(base,NAME);
#undef NAME
