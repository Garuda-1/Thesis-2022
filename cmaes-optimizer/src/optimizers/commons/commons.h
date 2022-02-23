#ifndef OPTIMIZER_COMMONS_H
#define OPTIMIZER_COMMONS_H

#include <string>
#include <iostream>
#include <algorithm>
#include <utility>

#include <boost/process.hpp>

namespace bp = boost::process;

namespace common {

inline void log(const std::string &message) {
    std::cout << "x " << message << '\n';
}

inline void log() {
    std::cout << "x \n";
}

struct cnf {
    size_t var_count = 0, cla_count = 0;
    std::vector<std::string> clauses;

    explicit cnf(std::string path_to_dimacs);

    void write_cnf_header(bp::opstream &solver_input) const;

    void write_cnf_activity(bp::opstream &solver_input, const std::vector<double> &activity) const;

    void write_cnf_clauses_and_close(bp::opstream &solver_input) const;
};

struct solver {
    std::string path;

    explicit solver(std::string path) : path(std::move(path)) {}

    size_t unsat_proof_size(const cnf &cnf, const std::string &path_to_storage,
                            const std::optional<std::vector<double>> &activity) const;
};

struct sample {
    std::vector<double> activity;
    size_t fitness = -1;

    explicit sample(const double *a, size_t var_count);

    void evaluate(const cnf &cnf, const std::string &path_to_solver, const std::string &path_to_storage);
};

//bool operator<(const sample &a, const sample &b) {
//    return a.fitness < b.fitness;
//}
//
//bool operator==(const sample &a, const sample &b) {
//    return a.fitness == b.fitness;
//}

}

#endif
