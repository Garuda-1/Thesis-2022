#include "sample.h"

#include <utility>
#include "solver.h"

common::sample::sample(const double* a, size_t var_count) : activity(var_count) {
  activity.emplace(std::vector<double>(var_count));
  activity->assign(a, a + var_count);
}

common::sample::sample(std::vector<double> activity) : activity(std::move(activity)) {}

void common::sample::evaluate(
    const cnf& cnf, const std::string& path_to_solver, const common::optimizer_options& options) {
  common::solver s(path_to_solver);
  solver_output = s.run_unsat(cnf, activity, options);
}
