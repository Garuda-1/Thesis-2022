#ifndef OPTIMIZER_COMMONS_H
#define OPTIMIZER_COMMONS_H

#include <string>
#include <iostream>
#include <algorithm>
#include <utility>

#include "sample.h"
#include "solver.h"

#include <boost/process.hpp>

namespace bp = boost::process;

namespace common {

inline void log(const std::string &message) {
    std::cout << "x " << message << '\n';
}

inline void log() {
    std::cout << "x \n";
}

struct hash_pair {
    template<class T1, class T2>
    size_t operator()(const std::pair<T1, T2> &p) const {
        return std::hash<T1>{}(p.first) ^ std::hash<T1>{}(p.second);
    }
};

}

#endif
