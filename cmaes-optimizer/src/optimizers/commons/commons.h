#ifndef OPTIMIZER_COMMONS_H
#define OPTIMIZER_COMMONS_H

#include <string>
#include <iostream>
#include <algorithm>
#include <utility>

#include "cnf.h"
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

//bool operator<(const sample &a, const sample &b) {
//    return a.fitness < b.fitness;
//}
//
//bool operator==(const sample &a, const sample &b) {
//    return a.fitness == b.fitness;
//}

}

#endif
