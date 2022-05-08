#include "cmaes/cmaes_optimizer.h"

#include <utility>

cmaes_optimizer::cmaes_optimizer(
    std::string path_to_solver, const std::string& path_to_dimacs, double base_value, double sigma, int32_t lambda,
    PGconn* pg_conn, int64_t experiment_id)
    : optimizer(std::move(path_to_solver), common::cnf(path_to_dimacs), pg_conn, experiment_id) {
  std::vector<double> activity(benchmark.var_count, base_value);
  cma_params = libcmaes::CMAParameters<>(activity, sigma, lambda);
  cma_params.set_max_iter(100);
}

ssize_t cmaes_optimizer::fit() {
  libcmaes::CMASolutions cma_solutions = libcmaes::cmaes<>(fit_function, cma_params);
  log_optimization_result();
  return static_cast<ssize_t>(cma_solutions.best_candidate().get_fvalue());
}

double cmaes_optimizer::evaluate(const double* a, [[maybe_unused]] size_t n) {
  common::sample sample(a, benchmark.var_count);
  common::optimizer_options options = {true, false, true};
  return static_cast<double>(evaluate_and_record(sample, options).proof_size);
}
