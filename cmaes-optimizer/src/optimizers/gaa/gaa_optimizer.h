#ifndef OPTIMIZER_GAA_OPTIMIZER_H
#define OPTIMIZER_GAA_OPTIMIZER_H

#include "commons/commons.h"
#include "commons/optimizer.h"
#include "commons/cnf_a.h"

#define OPENGA_EXTERN_LOCAL_VARS

#include <openGA.hpp>

struct gaa_optimizer : optimizer {
  EA::Genetic<common::cnf_a, common::sample> ga;

  std::mt19937 gen;

  gaa_optimizer(std::string path_to_solver, std::string path_to_dimacs, PGconn* pg_conn, int64_t experiment_id);

  void init_genes(common::cnf_a& cnf, const std::function<double(void)>& rnd);

  bool eval_solution(const common::cnf_a& cnf, common::sample& sample);

  common::cnf_a mutate(const common::cnf_a& base, const std::function<double(void)>& rnd, double shrink_scale);

  common::cnf_a crossover(const common::cnf_a& a, const common::cnf_a& b, const std::function<double(void)>& rnd);

  double calculate(const EA::Genetic<common::cnf_a, common::sample>::thisChromosomeType& x);

  void report_generation(
      int generation_number, const EA::GenerationType<common::cnf_a, common::sample>& last_generation,
      const common::cnf_a& best_genes);

  ssize_t fit() override;
};

#endif
