#ifndef OPTIMIZER_ONE_SHOT_OPTIMIZER_H
#define OPTIMIZER_ONE_SHOT_OPTIMIZER_H

#include "commons/optimizer.h"
#include "commons/dsu.h"
#include "commons/cnf_a.h"

#include <unordered_set>
#include <random>

struct one_shot_optimizer : optimizer {
  std::mt19937 gen;

  one_shot_optimizer(
      std::string path_to_solver, std::string path_to_dimacs, PGconn* pg_conn,
      int64_t experiment_id);

  ssize_t fit() override;

  ssize_t fit_p(
      double sample_rate, common::sample& sample, const common::optimizer_options& options,
      const std::vector<std::unordered_set<size_t>>& var_clauses_p,
      const std::vector<std::unordered_set<size_t>>& var_clauses_n);

  void add_er(int64_t lit_a, int64_t lit_b, common::cnf &cnf);
};

#endif  // OPTIMIZER_ONE_SHOT_OPTIMIZER_H
