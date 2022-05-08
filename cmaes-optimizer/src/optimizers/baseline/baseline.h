#ifndef OPTIMIZER_BASELINE_H
#define OPTIMIZER_BASELINE_H

#include "commons/optimizer.h"

struct baseline : optimizer {
  baseline(std::string path_to_solver, std::string path_to_dimacs, PGconn* pg_conn, int64_t experiment_id);

  ssize_t fit() override;
};

#endif
