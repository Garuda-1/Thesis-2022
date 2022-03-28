#include "optimizer.h"
#include "sample.h"
#include "commons.h"

ssize_t optimizer::evaluate_and_record(common::sample &sample, std::string &proof_file_path) {
    return evaluate_and_record(this->benchmark, sample, proof_file_path);
}

ssize_t optimizer::evaluate_and_record(const common::cnf &cnf, common::sample &sample, std::string &proof_file_path) {
    sample.evaluate(cnf, path_to_solver, path_to_storage, proof_file_path);
    if (best_fitness < 0 || sample.fitness < best_fitness) {
        best_fitness = sample.fitness;
    }
    common::log();
    common::log(std::to_string(sample.fitness) + " | " + std::to_string(static_cast<uint64_t>(best_fitness)) +
                " | " + std::to_string(iteration++));

    PGresult *result;
    std::string query = "INSERT INTO runs (experiment_id, proof_size) VALUES (" + std::to_string(experiment_id) + ", " +
                        std::to_string(sample.fitness) + ")";
    result = PQexec(pg_conn, query.c_str());
    ExecStatusType requestStatus = PQresultStatus(result);
    if (requestStatus == PGRES_COMMAND_OK) {
        common::log("Result recorded");
    } else {
        common::log("Failed to record result, status: " + std::to_string(requestStatus));
    }
    PQclear(result);
    common::log();

    return sample.fitness;
}

void optimizer::log_optimization_result() const {
    common::log("Optimization complete!");
    common::log("Best fitness: " + std::to_string(best_fitness));
}
