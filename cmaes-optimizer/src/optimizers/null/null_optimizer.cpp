#include "null_optimizer.h"

null_optimizer::null_optimizer(
    std::string path_to_solver, std::string path_to_dimacs, PGconn* pg_conn, int64_t experiment_id)
    : optimizer(std::move(path_to_solver), common::cnf(std::move(path_to_dimacs)), pg_conn, experiment_id) {}

ssize_t null_optimizer::fit() {
  for (size_t i = 0; optimizer::within_time_resources(); ++i) {
    ssize_t batch_best = std::numeric_limits<ssize_t>::max();
    for (size_t j = 0; j < BATCH_SIZE; ++j) {
      common::sample sample;
      common::optimizer_options options = {false, true, true, false};
      batch_best = std::min(batch_best, evaluate_and_record(sample, options).proof_size);
    }

    best = std::min(best, batch_best);
    common::record_run_to_db(pg_conn, experiment_id, best);
  }

  return best;
}
