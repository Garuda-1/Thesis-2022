#ifndef OPTIMIZER_CNF_H
#define OPTIMIZER_CNF_H

#include <vector>
#include <string>

#include <boost/process.hpp>

namespace bp = boost::process;

namespace common {

struct cnf {
    size_t var_count = 0, cla_count = 0;
    std::vector<std::vector<int64_t>> clauses;

    explicit cnf(std::string path_to_dimacs);

    void write_cnf_header(bp::opstream &solver_input) const;

    void write_cnf_activity(bp::opstream &solver_input, const std::vector<double> &activity) const;

    void write_cnf_clauses_and_close(bp::opstream &solver_input) const;
};

}

#endif
