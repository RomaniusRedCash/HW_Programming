#pragma once

#include <cstdint>
#include <vector>
#include <unordered_map>
#include "../bytebitstream.h"
#include "../sub_fun.h"

namespace std {
    template <>
    struct hash<bytebit> {
        size_t operator()(const bytebit& b) const {
            return hash<std::string>{}(b.get_data());
        }
    };
}

namespace itu_ns {
    class calculator {
    protected:
        bimap<uint8_t, bytebit> mapa;
        void add_to_map(const uint8_t& a, const uint8_t& size, const uint32_t& b);
    public:
        calculator() = default;
        virtual ~calculator() = default;
        const bytebit& operator[](const uint8_t& l) const;
        const uint8_t& operator[](const bytebit& bb) const;
        bool have_code(const bytebit& bb) const;
    };

    class calculator_DC : public calculator {
    public:
        calculator_DC();
    };
    class calculator_CDC : public calculator {
    public:
        calculator_CDC();
    };
    class calculator_AC : public calculator {
    public:
        calculator_AC();
    };
    class calculator_CAC : public calculator {
    public:
        calculator_CAC();
    };
}
