#include "solver.h"

ssize_t common::solver::unsat_proof_size(const cnf &cnf, const std::string &path_to_storage,
                                         const std::optional<std::vector<double>> &activity,
                                         std::string &proof_file_path) const {
    std::string unique_tag = boost::filesystem::unique_path().string();
    proof_file_path = path_to_storage + "/" + unique_tag + "-proof.dimacs";
    std::string log_file_path = path_to_storage + "/" + unique_tag + "-log.log";

    bp::opstream in;
    bp::child c(path + " -certified -certified-output=" + proof_file_path, bp::std_in < in,
                bp::std_out > log_file_path);
    cnf.write_cnf_header(in);
    if (activity) {
        cnf.write_cnf_activity(in, *activity);
    }
    cnf.write_cnf_clauses(in);
    in.close();
    in.pipe().close();
    c.wait();

    std::ifstream proof_stream(proof_file_path);
    ssize_t proof_size = 0;
    std::string line;
    while (std::getline(proof_stream, line)) {
        if (line[0] != 't') {
            ++proof_size;
        }
    }

    return proof_size;
}