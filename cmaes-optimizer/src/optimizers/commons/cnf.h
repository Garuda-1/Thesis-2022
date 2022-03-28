#ifndef OPTIMIZER_CNF_H
#define OPTIMIZER_CNF_H

#include "commons.h"

#include <vector>
#include <string>

#include <boost/process.hpp>

namespace bp = boost::process;

namespace common {

struct cnf {
    size_t var_count = 0, cla_count = 0;
    std::vector<std::vector<int64_t>> clauses;

    cnf() = default;

    explicit cnf(std::string path_to_dimacs);

    cnf(size_t var_count, size_t cla_count, std::vector<std::vector<int64_t>> clauses)
        : var_count(var_count), cla_count(cla_count), clauses(std::move(clauses)) {}

    void write_cnf_header(bp::opstream &solver_input) const;

    void write_cnf_activity(bp::opstream &solver_input, const std::vector<double> &activity) const;

    virtual void write_cnf_clauses(bp::opstream &solver_input) const;
};

}

#endif
