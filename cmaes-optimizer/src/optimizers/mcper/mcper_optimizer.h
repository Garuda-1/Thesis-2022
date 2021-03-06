#ifndef OPTIMIZER_MCPER_OPTIMIZER_H
#define OPTIMIZER_MCPER_OPTIMIZER_H

#include "commons/commons.h"
#include "commons/optimizer.h"

enum mcper_mode { TRAIL_FREQUENCIES, TRAIL_FREQUENCIES_PLUS, CONFLICT_FREQUENCIES, CONFLICT_FREQUENCIES_PLUS };

struct mcper_optimizer : optimizer {
  const size_t new_pairs_count;
  const double RESCALE_LIMIT = 1e100;
  double bump_factor = 1.25;
  std::vector<double> activity;
  mcper_mode mode;

  mcper_optimizer(
      size_t new_pairs_count, mcper_mode mode, std::string path_to_solver, std::string path_to_dimacs, PGconn* pg_conn,
      int64_t experiment_id);

  ssize_t fit() override;
};

#endif
