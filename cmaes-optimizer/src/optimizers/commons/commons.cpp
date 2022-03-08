#include <commons/commons.h>

#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <chrono>

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
        } else if (line.find("p cnf") == 0) {
            std::stringstream line_stream(line);
            std::string dummy;
            line_stream >> dummy >> dummy >> var_count >> cla_count;
        } else {
            clauses.push_back(line);
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
    for (const auto &s : clauses) {
        solver_input << s << '\n';
    }
    solver_input.close();
    solver_input.pipe().close();
}

size_t common::solver::unsat_proof_size(const cnf &cnf, const std::string &path_to_storage,
                                        const std::optional<std::vector<double>> &activity = {}) const {
    std::string unique_tag = boost::filesystem::unique_path().string();
    std::string proof_file_path = path_to_storage + "/" + unique_tag + "-proof.dimacs";
    std::string log_file_path = path_to_storage + "/" + unique_tag + "-log.log";

    bp::opstream in;
    bp::child c(path + " -certified -certified-output=" + proof_file_path, bp::std_in < in, bp::std_out > log_file_path);
    cnf.write_cnf_header(in);
    if (activity) {
        cnf.write_cnf_activity(in, *activity);
    }
    cnf.write_cnf_clauses_and_close(in);
    c.wait();

    std::ifstream proof_stream(proof_file_path);
    size_t proof_size = 0;
    std::string line;
    while (std::getline(proof_stream, line)) {
        ++proof_size;
    }
    return proof_size;
}

common::sample::sample(const double *a, size_t var_count) : activity(var_count) {
    activity.assign(a, a + var_count);
}

void common::sample::evaluate(const cnf &cnf, const std::string &path_to_solver, const std::string &path_to_storage) {
    common::solver s(path_to_solver);
    fitness = s.unsat_proof_size(cnf, path_to_storage);
}
