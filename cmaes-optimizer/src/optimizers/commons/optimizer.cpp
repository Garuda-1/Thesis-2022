#include "optimizer.h"
#include "sample.h"
#include "commons.h"

ssize_t optimizer::evaluate_and_record(common::sample &sample, std::string &proof_file_path, bool record_result_to_db, bool rnd_init) {
    return evaluate_and_record(this->benchmark, sample, proof_file_path, record_result_to_db, rnd_init);
}

ssize_t optimizer::evaluate_and_record(const common::cnf &cnf, common::sample &sample, std::string &proof_file_path, bool record_result_to_db, bool rnd_init) {
    sample.evaluate(cnf, path_to_solver, path_to_storage, proof_file_path, rnd_init);
    if (best_fitness < 0 || sample.fitness < best_fitness) {
        best_fitness = sample.fitness;
    }
    common::log();
    common::log(std::to_string(sample.fitness) + " | " + std::to_string(static_cast<uint64_t>(best_fitness)) +
                " | " + std::to_string(iteration++));
    if (record_result_to_db) {
        common::record_run_to_db(pg_conn, experiment_id, sample.fitness);
    }
    common::log();

    return sample.fitness;
}

void optimizer::log_optimization_result() const {
    common::log("Optimization complete!");
    common::log("Best fitness: " + std::to_string(best_fitness));
}

void optimizer::clear_logs() const {
    boost::filesystem::remove_all(path_to_storage);
}
