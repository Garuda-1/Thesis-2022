#include "eaer_optimizer.h"

eaer_optimizer::eaer_optimizer(
    std::string path_to_solver, std::string path_to_storage, std::string path_to_dimacs, PGconn* pg_conn,
    int64_t experiment_id)
    : cnf(common::cnf(std::move(path_to_dimacs)))
    , optimizer(
          std::move(path_to_solver), std::move(path_to_storage), common::cnf(std::move(path_to_dimacs)), pg_conn,
          experiment_id) {
  genome.resize(cnf.var_count, std::vector<bool>(cnf.var_count));
  flip = std::bernoulli_distribution(pow(static_cast<double>(cnf.var_count), -2));
  gen = std::mt19937(random_device());
}

void eaer_optimizer::init_genome() {
  for (size_t i = 0; i < genome.size() - 1; ++i) {
    for (size_t j = i + 1; j < genome.size(); ++j) {
      genome[i][j] = flip(gen);
      if (genome[i][j]) {
        cnf.er_pairs.insert({i, j});
      }
    }
  }
  common::sample sample;
  best = evaluate_and_record(cnf, sample, OPTIONS).proof_size;
  common::log("Starting with " + std::to_string(best) + " conflicts");
}

void eaer_optimizer::mutate() {
  std::unordered_set<std::pair<int64_t, int64_t>, common::hash_pair> er_pairs_delta_plus;
  std::unordered_set<std::pair<int64_t, int64_t>, common::hash_pair> er_pairs_delta_minus;
  for (size_t i = 0; i < genome.size() - 1; ++i) {
    for (size_t j = i + 1; j < genome.size(); ++j) {
      if (flip(gen)) {
        if (genome[i][j]) {
          er_pairs_delta_minus.insert({i, j});
          cnf.er_pairs.erase(cnf.er_pairs.find({i, j}));
        } else {
          er_pairs_delta_plus.insert({i, j});
          cnf.er_pairs.insert({i, j});
        }
        genome[i][j] = !genome[i][j];
      }
    }
  }
  common::log(
      "Mutated: +" + std::to_string(er_pairs_delta_plus.size()) + " -" + std::to_string(er_pairs_delta_minus.size()));
  common::sample sample;
  common::optimizer_options options = {false, false, true, false};

  ssize_t new_proof_size = evaluate_and_record(cnf, sample, options).proof_size;
  if (new_proof_size < best) {
    common::log("Improvement: " + std::to_string(best) + " -> " + std::to_string(new_proof_size));
    best = new_proof_size;
  } else {
    for (const auto &p : er_pairs_delta_minus) {
      cnf.er_pairs.insert(p);
      genome[p.first][p.second] = !genome[p.first][p.second];
    }
    for (const auto &p : er_pairs_delta_minus) {
      cnf.er_pairs.erase(cnf.er_pairs.find(p));
      genome[p.first][p.second] = !genome[p.first][p.second];
    }
  }
}

ssize_t eaer_optimizer::fit() {
  init_genome();
  for (size_t i = 0; i < MAX_ITERATIONS * BATCH_SIZE && optimizer::within_time_resources(); ++i) {
    mutate();
    if (i % BATCH_SIZE == 0) {
      common::record_run_to_db(pg_conn, experiment_id, best);
    }
  }
  return best;
}
