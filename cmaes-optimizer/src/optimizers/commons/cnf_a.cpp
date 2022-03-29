#include "cnf_a.h"

void common::cnf_a::write_cnf_header(bp::opstream &solver_input) const {
    cnf::write_cnf_header(solver_input);
    solver_input << "activity ";
    solver_input << std::scientific;
    for (double a : activity) {
        solver_input << a << " ";
    }
    solver_input << "\n";
}
