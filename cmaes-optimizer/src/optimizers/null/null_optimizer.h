#ifndef OPTIMIZER_NULL_OPTIMIZER_H
#define OPTIMIZER_NULL_OPTIMIZER_H

#include "commons/optimizer.h"

struct null_optimizer : optimizer {
  const size_t MAX_ITERATIONS = 201;
  const size_t BATCH_SIZE = 20;
  ssize_t best = std::numeric_limits<ssize_t>::max();

  null_optimizer(
      std::string path_to_solver, std::string path_to_storage, std::string path_to_dimacs, PGconn* pg_conn,
      int64_t experiment_id);

  ssize_t fit() override;
};

#endif
