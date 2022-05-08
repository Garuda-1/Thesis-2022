#ifndef OPTIMIZER_CMAES_OPTIMIZER_H
#define OPTIMIZER_CMAES_OPTIMIZER_H

#include "commons/commons.h"
#include "commons/optimizer.h"

#include <string>

#include <libcmaes/cmaes.h>
#include <libpq-fe.h>

struct cmaes_optimizer : optimizer {
  libcmaes::CMAParameters<> cma_params;

  libcmaes::FitFunc fit_function = [=](const double* a, const size_t n) { return evaluate(a, n); };

  cmaes_optimizer(
      std::string path_to_solver, const std::string& path_to_dimacs, double base_value = 1, double sigma = 0.2,
      int32_t lambda = -1, PGconn* pg_conn = nullptr, int64_t experiment_id = -1);

  ssize_t fit() override;

  double evaluate(const double* a, [[maybe_unused]] size_t n);
};

#endif
