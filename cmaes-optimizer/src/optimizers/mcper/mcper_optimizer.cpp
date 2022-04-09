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
        common::sample sample(activity);
        common::optimizer_options options = {true, false, false, false};
        common::solver_output output = evaluate_and_record(sample, options);

        std::vector<std::pair<std::pair<int64_t, int64_t>, size_t>> top_pairs(TOP_PAIRS_COUNT);
        std::partial_sort_copy(output.trail_frequencies.begin(), output.trail_frequencies.end(), top_pairs.begin(), top_pairs.end(),
                               [](std::pair<std::pair<size_t, size_t>, size_t> const &l,
                                  std::pair<std::pair<size_t, size_t>, size_t> const &r) {
                                   return l.second > r.second;
                               });

        for (const auto &top_pair: top_pairs) {
            int64_t lit_a = top_pair.first.first;
            int64_t lit_b = top_pair.first.second;
            size_t frequency = top_pair.second;
            size_t var_x = benchmark.var_count + 1;
            common::log("Extending resolution with variables " + std::to_string(lit_a) + " and " +
                        std::to_string(lit_a) + " , frequency: " + std::to_string(frequency) +
                        " , new variable name: " +
                        std::to_string(var_x));

            auto var_a = static_cast<size_t>(std::abs(lit_a));
            auto var_b = static_cast<size_t>(std::abs(lit_b));
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
