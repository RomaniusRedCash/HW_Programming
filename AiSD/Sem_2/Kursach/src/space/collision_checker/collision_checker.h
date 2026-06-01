#pragma once

#include <iostream>

#include "../../object/rectangle/rectangle.h"
#include "../../object/line/line.h"
// #include "../../object/rectangle/rectangle.h"

class collision_checker {
public:
    bool operator()(const rectangle &rec, const line &ln) const ;
    bool operator()(const line &ln, const rectangle &rec) const {
        return (*this)(rec, ln);
    }
    bool operator()(const shape &shp1, const shape &shp2) const;

    template <typename T, typename U>
    bool operator()(const T &, const U &) const {
        std::cerr << "ERROR! can't check collision.";
        throw "ERROR!";
    }
};
