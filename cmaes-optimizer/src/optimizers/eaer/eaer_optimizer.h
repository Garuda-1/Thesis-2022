#ifndef OPTIMIZER_EAER_OPTIMIZER_H
#define OPTIMIZER_EAER_OPTIMIZER_H

#include "commons/commons.h"
#include "commons/optimizer.h"
#include "commons/cnf_er.h"

#include <random>

struct eaer_optimizer : optimizer {

  const size_t MAX_ITERATIONS = 201;
  const size_t BATCH_SIZE = 20;
  const common::optimizer_options OPTIONS = {false, false, true, false};
  std::vector<std::vector<bool>> genome;
  std::bernoulli_distribution flip;
  common::cnf_er cnf;
  ssize_t best = std::numeric_limits<ssize_t>::max();
  std::random_device random_device;
  std::mt19937 gen;


  eaer_optimizer(
      std::string path_to_solver, std::string path_to_storage, const std::string &path_to_dimacs, PGconn* pg_conn,
      int64_t experiment_id);

  void init_genome();

  void mutate();

  ssize_t fit() override;
};

#endif
