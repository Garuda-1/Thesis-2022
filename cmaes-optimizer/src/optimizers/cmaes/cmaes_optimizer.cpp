#include "cmaes/cmaes_optimizer.h"

#include <utility>
#include "commons/commons.h"

cmaes_optimizer::cmaes_optimizer(std::string path_to_solver, const std::string &path_to_dimacs,
                                 std::string path_to_storage, double base_value, double sigma, int32_t lambda)
    : path_to_solver(std::move(path_to_solver)), path_to_storage(std::move(path_to_storage)), benchmark(path_to_dimacs) {
    std::vector<double> activity(benchmark.var_count, base_value);
    cmaparams = libcmaes::CMAParameters<>(activity, sigma, lambda);
}

size_t cmaes_optimizer::fit() {
    libcmaes::CMASolutions cmasols = libcmaes::cmaes<>(fit_function, cmaparams);
    common::log("Optimization complete!");
    common::log("Best fitness: " + std::to_string(cmasols.best_candidate().get_fvalue()));
    return static_cast<size_t>(cmasols.best_candidate().get_fvalue());
}

double cmaes_optimizer::evaluate(const double *a, size_t n) {
    common::sample sample(a, benchmark.var_count);
    sample.evaluate(benchmark, path_to_solver, path_to_storage);
    if (best_fitness < 0 || sample.fitness < best_fitness) {
        best_fitness = sample.fitness;
    }
    common::log();
    common::log(std::to_string(sample.fitness) + " | " + std::to_string(static_cast<uint64_t>(best_fitness)) +
        " | " + std::to_string(iteration++));
    common::log();
    return sample.fitness;
}
