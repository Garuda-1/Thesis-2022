#ifndef OPTIMIZER_CNF_ER_H
#define OPTIMIZER_CNF_ER_H

#include "cnf.h"

#include <utility>
#include <vector>
#include <string>
#include <unordered_set>

#include <boost/process.hpp>

namespace bp = boost::process;

namespace common {

struct cnf_er : cnf {

    std::unordered_set<std::pair<int64_t, int64_t>, common::hash_pair> er_pairs;

    cnf_er() = default;

    cnf_er(const cnf_er &cnf_er) = default;

    explicit cnf_er(const cnf& cnf) : cnf_er(cnf.var_count, cnf.cla_count, cnf.clauses) {}

    cnf_er(size_t var_count, size_t cla_count, std::vector<std::vector<int64_t>> clauses)
        : cnf(var_count, cla_count, std::move(clauses)) {}

    void write_cnf_header(bp::opstream &solver_input) const override;

    void write_cnf_clauses(bp::opstream &solver_input) const override;
};

}

#endif
