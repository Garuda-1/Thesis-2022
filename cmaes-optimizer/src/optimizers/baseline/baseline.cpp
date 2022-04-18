#include "baseline.h"

baseline::baseline(
    std::string path_to_solver, std::string path_to_storage, std::string path_to_dimacs, PGconn* pg_conn,
    int64_t experiment_id)
    : optimizer(
          std::move(path_to_solver), std::move(path_to_storage), common::cnf(std::move(path_to_dimacs)), pg_conn,
          experiment_id) {}

ssize_t baseline::fit() {
  common::optimizer_options options = {true, false, true, false};
  common::sample sample;
  ssize_t result = evaluate_and_record(sample, options).proof_size;
  return result;
}
