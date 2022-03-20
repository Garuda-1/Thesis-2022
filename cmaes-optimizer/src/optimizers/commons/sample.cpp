#include "sample.h"

#include <utility>
#include "solver.h"

common::sample::sample(const double *a, size_t var_count) : activity(var_count) {
    activity.assign(a, a + var_count);
}

common::sample::sample(std::vector<double> activity) : activity(std::move(activity)) {}

void common::sample::evaluate(const cnf &cnf, const std::string &path_to_solver, const std::string &path_to_storage,
                              std::string &proof_file_path) {
    common::solver s(path_to_solver);
    fitness = s.unsat_proof_size(cnf, path_to_storage, std::make_optional(activity), proof_file_path);
}
