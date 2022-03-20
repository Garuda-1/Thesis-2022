#include "commons.h"
#include "cnf.h"

#include <sstream>

common::cnf::cnf(std::string path_to_dimacs) {
    if (!boost::filesystem::exists(path_to_dimacs)) {
        throw std::runtime_error("Benchmark file '" + path_to_dimacs +"' does not exist");
    }

    log("Starting read of benchmark '" + path_to_dimacs + "'...");

    std::ifstream input_stream(path_to_dimacs);
    std::string line;
    while (std::getline(input_stream, line)) {
        if (line[0] == 'c') {
            continue;
        } else {
            std::stringstream line_stream(line);
            if (line.find("p cnf") == 0) {
                std::string dummy;
                line_stream >> dummy >> dummy >> var_count >> cla_count;
            } else {
                std::vector<int64_t> lits;
                for(;;) {
                    int64_t lit;
                    line_stream >> lit;
                    if (lit == 0) {
                        break;
                    }
                    lits.push_back(lit);
                }
                clauses.push_back(lits);
            }
        }
    }

    log("Benchmark read complete: " + std::to_string(var_count) + " variables, " +
        std::to_string(cla_count) + " clauses");
}

void common::cnf::write_cnf_header(bp::opstream &solver_input) const {
    solver_input << "p cnf " << var_count << " " << cla_count << '\n';
}

void common::cnf::write_cnf_activity(bp::opstream &solver_input, const std::vector<double> &activity) const {
    solver_input << std::scientific;
    solver_input << "activity ";
    for (const auto &a : activity) {
        solver_input << a << ' ';
    }
    solver_input << '\n';
}

void common::cnf::write_cnf_clauses_and_close(bp::opstream &solver_input) const {
    for (const auto &lits : clauses) {
        for (const auto &lit : lits) {
            solver_input << lit << ' ';
        }
        solver_input << "0\n";
    }
    solver_input.close();
    solver_input.pipe().close();
}
