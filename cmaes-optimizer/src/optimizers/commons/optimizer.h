#ifndef OPTIMIZER_OPTIMIZER_H
#define OPTIMIZER_OPTIMIZER_H

#include "cnf.h"
#include "sample.h"

#include <libpq-fe.h>

#include <chrono>
#include <utility>

struct optimizer {
  const size_t MAX_HOURS = 8;

  std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();

  common::cnf benchmark;

  ssize_t best_fitness = -1;
  size_t iteration = 0;

  const std::string path_to_solver;
  const std::string path_to_storage;
  const int64_t experiment_id;

  PGconn* pg_conn;

  optimizer(
      std::string path_to_solver, std::string path_to_storage, common::cnf benchmark, PGconn* pg_conn,
      int64_t experiment_id)
      : path_to_solver(std::move(path_to_solver))
      , path_to_storage(std::move(path_to_storage))
      , benchmark(std::move(benchmark))
      , pg_conn(pg_conn)
      , experiment_id(experiment_id) {}

  virtual ~optimizer() = default;

  virtual ssize_t fit() = 0;

  common::solver_output evaluate_and_record(common::sample& sample, const common::optimizer_options& options);

  common::solver_output evaluate_and_record(
      const common::cnf& cnf, common::sample& sample, const common::optimizer_options& options);

  void log_optimization_result() const;

  void clear_logs() const;

  bool within_time_resources() const;
};

#endif
