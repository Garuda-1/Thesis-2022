#include "one_shot_optimizer.h"

one_shot_optimizer::one_shot_optimizer(
    std::string path_to_solver, std::string path_to_storage, std::string path_to_dimacs, PGconn* pg_conn,
    int64_t experiment_id)
    : optimizer(
          std::move(path_to_solver), std::move(path_to_storage), common::cnf(std::move(path_to_dimacs)), pg_conn,
          experiment_id) {}

ssize_t one_shot_optimizer::fit() {
  common::optimizer_options options = {true, false, true, false};
  common::sample sample;

  evaluate_and_record(sample, options);

  std::vector<std::unordered_set<size_t>> var_clauses_p(benchmark.var_count + 1);
  std::vector<std::unordered_set<size_t>> var_clauses_n(benchmark.var_count + 1);
  for (size_t i = 0; i < benchmark.cla_count; ++i) {
    for (const auto& var : benchmark.clauses[i]) {
      if (var > 0) {
        var_clauses_p[var].insert(i);
      } else {
        var_clauses_n[-var].insert(i);
      }
    }
  }

  std::vector<double> sample_rates = {0.001, 0.0025, 0.005, 0.0075, 0.01, 0.025, 0.05, 0.075, 0.1};
  ssize_t result = std::numeric_limits<ssize_t>::max();

  for (double rate : sample_rates) {
    result = std::min(result, fit_p(rate, sample, options, var_clauses_p, var_clauses_n));
  }

  return result;
}

ssize_t one_shot_optimizer::fit_p(
    double sample_rate, common::sample& sample, const common::optimizer_options& options,
    const std::vector<std::unordered_set<size_t>>& var_clauses_p,
    const std::vector<std::unordered_set<size_t>>& var_clauses_n) {
  std::bernoulli_distribution dist(sample_rate);

  common::cnf cnf = benchmark;

  size_t base_var_count = cnf.var_count;
  size_t new_pairs_count = 0;
  for (size_t var_a = 1; var_a <= base_var_count; ++var_a) {
    for (size_t var_b = 1; var_b <= base_var_count; ++var_b) {
      for (const auto& c : var_clauses_p[var_a]) {
        if (var_clauses_p[var_b].find(c) != var_clauses_p[var_b].end() && dist(gen) &&
            new_pairs_count < base_var_count) {
          add_er(static_cast<int64_t>(var_a), static_cast<int64_t>(var_b), cnf);
          ++new_pairs_count;
        }
        if (var_clauses_n[var_b].find(c) != var_clauses_n[var_b].end() && dist(gen) &&
            new_pairs_count < base_var_count) {
          add_er(static_cast<int64_t>(var_a), -static_cast<int64_t>(var_b), cnf);
          ++new_pairs_count;
        }
      }
    }
  }
  for (size_t var_a = 1; var_a <= base_var_count; ++var_a) {
    for (size_t var_b = 1; var_b <= base_var_count; ++var_b) {
      for (const auto& c : var_clauses_n[var_a]) {
        if (var_clauses_p[var_b].find(c) != var_clauses_p[var_b].end() && dist(gen) &&
            new_pairs_count < base_var_count) {
          add_er(-static_cast<int64_t>(var_a), static_cast<int64_t>(var_b), cnf);
          ++new_pairs_count;
        }
        if (var_clauses_n[var_b].find(c) != var_clauses_n[var_b].end() && dist(gen) &&
            new_pairs_count < base_var_count) {
          add_er(-static_cast<int64_t>(var_a), -static_cast<int64_t>(var_b), cnf);
          ++new_pairs_count;
        }
      }
    }
  }

  common::log("Added " + std::to_string(new_pairs_count) + " pairs");

  ssize_t result = evaluate_and_record(cnf, sample, options).proof_size;

  return result;
}

void one_shot_optimizer::add_er(int64_t lit_a, int64_t lit_b, common::cnf& cnf) {
  auto lit_x = static_cast<int64_t>(cnf.var_count + 1);
  ++cnf.var_count;
  cnf.cla_count += 3;
  cnf.clauses.push_back({lit_x, -lit_a, -lit_b});
  cnf.clauses.push_back({-lit_x, lit_a});
  cnf.clauses.push_back({-lit_x, lit_b});
}
