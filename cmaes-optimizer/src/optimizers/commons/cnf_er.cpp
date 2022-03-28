#include "cnf_er.h"

void common::cnf_er::write_cnf_header(boost::process::opstream &solver_input) const {
    solver_input << "p cnf " << var_count + er_pairs.size() << " " << clauses.size() + er_pairs.size() * 3 << '\n';
}

void common::cnf_er::write_cnf_clauses(bp::opstream &solver_input) const {
    cnf::write_cnf_clauses(solver_input);

    auto new_var = static_cast<int64_t>(var_count);
    for (const auto &p : er_pairs) {
        ++new_var;
        if (new_var > var_count + er_pairs.size()) {
            throw std::runtime_error("Invalid new var name");
        }
        solver_input << new_var << ' ' << -p.first << ' ' << -p.second << " 0\n";
        solver_input << -new_var << ' ' << p.first << " 0\n";
        solver_input << -new_var << ' ' << p.second << " 0\n";
    }
}
