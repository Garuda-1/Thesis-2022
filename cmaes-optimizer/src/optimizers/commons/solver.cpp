#include "solver.h"

ssize_t common::solver::unsat_proof_size(const cnf &cnf, const std::string &path_to_storage,
                                         const std::optional<std::vector<double>> &activity,
                                         std::string &proof_file_path,
                                         bool rnd_init) const {
    std::string unique_tag = boost::filesystem::unique_path().string();
    proof_file_path = path_to_storage + "/" + unique_tag + "-proof.dimacs";
    std::string log_file_path = path_to_storage + "/" + unique_tag + "-log.log";

    std::random_device random_device;
    std::mt19937 random_gen(random_device());
    std::uniform_int_distribution distribution(0, 10000);

    bp::opstream in;
    std::string command = path + " -certified -certified-output=" + proof_file_path;
    if (rnd_init) {
        command += " -rnd-init -rnd-seed=" + std::to_string(distribution(random_gen));
    }
    bp::child c(command, bp::std_in < in, bp::std_out > log_file_path);
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