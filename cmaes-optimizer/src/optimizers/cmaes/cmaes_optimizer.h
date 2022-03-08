#ifndef OPTIMIZER_CMAES_OPTIMIZER_H
#define OPTIMIZER_CMAES_OPTIMIZER_H

#include <string>

#include <libcmaes/cmaes.h>
#include <libpq-fe.h>

#include "commons/commons.h"

struct cmaes_optimizer {
    const common::cnf benchmark;
    const std::string path_to_solver;
    const std::string path_to_storage;

    PGconn *pg_conn;
    const int64_t experiment_id;

    libcmaes::CMAParameters<> cmaparams;
    double best_fitness = -1;
    size_t iteration = 0;

    libcmaes::FitFunc fit_function = [=](const double *a, const size_t n) {
        return evaluate(a, n);
    };

    explicit cmaes_optimizer(std::string path_to_solver, const std::string &path_to_dimacs,
                             std::string path_to_storage,
                             double base_value = 1, double sigma = 0.2, int32_t lambda = -1,
                             PGconn *pg_conn = nullptr, int64_t experiment_id = -1);

    size_t fit();

    double evaluate(const double *a, const size_t n);
};


#endif
