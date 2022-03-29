#ifndef OPTIMIZER_CNF_A_H
#define OPTIMIZER_CNF_A_H

#include "cnf.h"

#include <vector>

#include <boost/process.hpp>

namespace bp = boost::process;

namespace common {

struct cnf_a : cnf {

    std::vector<double> activity;

    cnf_a() = default;

    cnf_a(const cnf_a &cnf_a) = default;

    explicit cnf_a(const cnf& cnf) : cnf_a(cnf.var_count, cnf.cla_count, cnf.clauses) {}

    cnf_a(size_t var_count, size_t cla_count, std::vector<std::vector<int64_t>> clauses)
        : cnf(var_count, cla_count, std::move(clauses)) {}

    void write_cnf_header(bp::opstream &solver_input) const override;
};

}


#endif //OPTIMIZER_CNF_A_H
