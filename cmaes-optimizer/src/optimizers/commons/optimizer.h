#ifndef OPTIMIZER_OPTIMIZER_H
#define OPTIMIZER_OPTIMIZER_H

#include "cnf.h"
#include "sample.h"

#include <libpq-fe.h>

#include <utility>

struct optimizer {
    common::cnf benchmark;

    ssize_t best_fitness = -1;
    size_t iteration = 0;

    const std::string path_to_solver;
    const std::string path_to_storage;
    const int64_t experiment_id;

    PGconn *pg_conn;

    optimizer(std::string path_to_solver, std::string path_to_storage, common::cnf benchmark, PGconn *pg_conn,
              int64_t experiment_id) :
            path_to_solver(std::move(path_to_solver)), path_to_storage(std::move(path_to_storage)),
            benchmark(std::move(benchmark)), pg_conn(pg_conn), experiment_id(experiment_id) {}

    virtual ~optimizer() = default;

    virtual ssize_t fit() = 0;

    ssize_t evaluate_and_record(common::sample &sample, std::string &proof_file_path);

    ssize_t evaluate_and_record(const common::cnf &cnf, common::sample &sample, std::string &proof_file_path);

    void log_optimization_result() const;
};

#endif
