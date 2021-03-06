#include "one_shot_optimizer.h"

one_shot_optimizer::one_shot_optimizer(
    std::string path_to_solver, std::string path_to_dimacs, PGconn* pg_conn,
    int64_t experiment_id)
    : optimizer(
          std::move(path_to_solver), common::cnf(std::move(path_to_dimacs)), pg_conn,
          experiment_id) {}

ssize_t one_shot_optimizer::fit() {
  common::optimizer_options options = {true, false, true, false};
  common::sample sample;

  evaluate_and_record(sample, options);

//  std::vector<std::unordered_set<size_t>> var_clauses_p(benchmark.var_count + 1);
//  std::vector<std::unordered_set<size_t>> var_clauses_n(benchmark.var_count + 1);
//  for (size_t i = 0; i < benchmark.cla_count; ++i) {
//    for (const auto& var : benchmark.clauses[i]) {
//      if (var > 0) {
//        var_clauses_p[var].insert(i);
//      } else {
//        var_clauses_n[-var].insert(i);
//      }
//    }
//  }
//
//  std::vector<double> sample_rates = {0.001, 0.0025, 0.005, 0.0075, 0.01, 0.025, 0.05, 0.075, 0.1};
//  ssize_t result = std::numeric_limits<ssize_t>::max();
//
//  for (double rate : sample_rates) {
//    result = std::min(result, fit_p(rate, sample, options, var_clauses_p, var_clauses_n));
//  }
  common::dsu dsu(benchmark.cla_count);
  common::cnf_a cnf(benchmark);
  cnf.activity.resize(cnf.var_count, 1e-10);
  std::vector<std::pair<size_t, size_t>> disjoint;

  for (size_t cla_i = 0; cla_i < benchmark.cla_count; ++cla_i) {
    for (size_t cla_j = cla_i + 1; cla_j < benchmark.cla_count; ++cla_j) {
      bool intersects = false;
      for (const auto lit_a : benchmark.clauses[cla_i]) {
        for (const auto lit_b : benchmark.clauses[cla_j]) {
          if (std::abs(lit_a) == std::abs(lit_b)) {
            intersects = true;
          }
        }
      }
      if (intersects) {
        dsu.union_sets(cla_i, cla_j);
      } else {
        disjoint.emplace_back(cla_i, cla_j);
      }
    }
  }

  size_t new_pairs_cnt = 0;
  std::bernoulli_distribution dist(static_cast<double>(cnf.cla_count) / disjoint.size());

  for (const auto &p : disjoint) {
    if (!dist(gen)) {
      continue;
    }

    auto lit_x = static_cast<int64_t>(cnf.var_count + 1);
    auto lit_a = cnf.clauses[p.first][0];
    auto lit_b = cnf.clauses[p.second][0];
    ++cnf.var_count;
    cnf.cla_count += 3;
    cnf.clauses.push_back({lit_x, -lit_a, -lit_b});
    cnf.clauses.push_back({-lit_x, lit_a});
    cnf.clauses.push_back({-lit_x, lit_b});
    cnf.activity.push_back(1);

    dsu.union_sets(p.first, p.second);
    ++new_pairs_cnt;
  }

  common::log("Added " + std::to_string(new_pairs_cnt) + " pairs");

  ssize_t result = evaluate_and_record(cnf, sample, options).proof_size;

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
