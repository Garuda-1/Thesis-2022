#include "mcper_optimizer.h"

#include <sstream>
#include <utility>

mcper_optimizer::mcper_optimizer(std::string path_to_solver, std::string path_to_storage,
                                 std::string path_to_dimacs, PGconn *pgConn, int64_t experimentId) :
        optimizer(std::move(path_to_solver), std::move(path_to_storage), common::cnf(std::move(path_to_dimacs)),
                  pgConn, experimentId) {
    activity.resize(benchmark.var_count, 1.0);
}

ssize_t mcper_optimizer::fit() {
    for (size_t iteration = 0; iteration < MAX_ITERATIONS; ++iteration) {
        std::string proof_file_path;
        common::sample sample(activity);
        evaluate_and_record(sample, proof_file_path);

        std::ifstream proof_stream(proof_file_path);
        std::unordered_map<std::pair<size_t, size_t>, size_t, common::hash_pair> pairs_count;
        std::string line;
        while (std::getline(proof_stream, line)) {
            if (line[0] == 't') {
                std::vector<size_t> trail;

                std::stringstream trail_stream(line);
                std::string dummy;
                trail_stream >> dummy;
                for (;;) {
                    size_t lit;
                    trail_stream >> lit;
                    if (lit == 0) {
                        break;
                    }
                    trail.push_back(lit);
                }

                for (size_t i = 0; i < trail.size() - 1; ++i) {
                    for (size_t j = i + 1; j < trail.size(); ++j) {
                        size_t var_a = trail[i];
                        size_t var_b = trail[j];
                        if (pairs_count.find({var_a, var_b}) == pairs_count.end()) {
                            pairs_count.insert({{var_a, var_b}, 0});
                        }
                        ++pairs_count[{var_a, var_b}];
                    }
                }
            }
        }

        std::vector<std::pair<std::pair<size_t, size_t>, size_t>> top_pairs(TOP_PAIRS_COUNT);
        std::partial_sort_copy(pairs_count.begin(), pairs_count.end(), top_pairs.begin(), top_pairs.end(),
                               [](std::pair<std::pair<size_t, size_t>, size_t> const &l,
                                  std::pair<std::pair<size_t, size_t>, size_t> const &r) {
                                   return l.second > r.second;
                               });

        for (const auto &top_pair: top_pairs) {
            size_t var_a = top_pair.first.first;
            size_t var_b = top_pair.first.second;
            size_t frequency = top_pair.second;
            size_t var_x = benchmark.var_count + 1;
            common::log("Extending resolution with variables " + std::to_string(var_a) + " and " +
                        std::to_string(var_b) + " , frequency: " + std::to_string(frequency) +
                        " , new variable name: " +
                        std::to_string(var_x));

            auto lit_a = static_cast<int64_t>(var_a);
            auto lit_b = static_cast<int64_t>(var_b);
            auto lit_x = static_cast<int64_t>(var_x);
            ++benchmark.var_count;
            benchmark.cla_count += 3;
            benchmark.clauses.push_back({lit_x, -lit_a, -lit_b});
            benchmark.clauses.push_back({-lit_x, lit_a});
            benchmark.clauses.push_back({-lit_x, lit_b});

            activity[var_a] += BUMP_FACTOR;
            activity[var_b] += BUMP_FACTOR;
            activity.push_back((activity[var_a] + activity[var_b]) / 2);

            if (std::max(activity[var_a], activity[var_b]) >= RESCALE_LIMIT) {
                for (auto &a : activity) {
                    a /= RESCALE_LIMIT;
                }
            }
        }
    }

    return best_fitness;
}
