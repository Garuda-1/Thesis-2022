#include "optimizer.h"
#include "sample.h"
#include "commons.h"

common::solver_output optimizer::evaluate_and_record(common::sample &sample, common::optimizer_options &options) {
    return evaluate_and_record(this->benchmark, sample, options);
}

common::solver_output optimizer::evaluate_and_record(const common::cnf &cnf, common::sample &sample, common::optimizer_options &options) {
    sample.evaluate(cnf, path_to_solver, path_to_storage, options);
    ssize_t sample_fitness = sample.solver_output.proof_size;
    if (best_fitness < 0 || sample_fitness < best_fitness) {
        best_fitness = sample_fitness;
    }
    common::log();
    common::log(std::to_string(sample_fitness) + " | " + std::to_string(static_cast<uint64_t>(best_fitness)) +
                " | " + std::to_string(iteration++));
    if (options.record_result_to_db) {
        common::record_run_to_db(pg_conn, experiment_id, sample_fitness);
    }
    common::log();

    return sample.solver_output;
}

void optimizer::log_optimization_result() const {
    common::log("Optimization complete!");
    common::log("Best fitness: " + std::to_string(best_fitness));
}

void optimizer::clear_logs() const {
    boost::filesystem::remove_all(path_to_storage);
}

bool optimizer::within_time_resources() const {
    auto time_hours = std::chrono::duration_cast<std::chrono::hours>(std::chrono::high_resolution_clock::now() - start);
    return time_hours.count() < MAX_HOURS;
//    auto time_seconds = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - start);
//    return time_seconds.count() < 10;
}
