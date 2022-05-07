#ifndef OPTIMIZER_ONE_SHOT_OPTIMIZER_H
#define OPTIMIZER_ONE_SHOT_OPTIMIZER_H

#include "commons/optimizer.h"

#include <unordered_set>

struct one_shot_optimizer : optimizer {
  one_shot_optimizer(
      std::string path_to_solver, std::string path_to_storage, std::string path_to_dimacs, PGconn* pg_conn,
      int64_t experiment_id);

  ssize_t fit() override;

  void add_er(int64_t lit_a, int64_t lit_b);
};

#endif  // OPTIMIZER_ONE_SHOT_OPTIMIZER_H
