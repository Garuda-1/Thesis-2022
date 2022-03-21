#ifndef OPTIMIZER_MCPER_OPTIMIZER_H
#define OPTIMIZER_MCPER_OPTIMIZER_H

#include "commons/commons.h"
#include "commons/optimizer.h"

struct mcper_optimizer : optimizer {
    const size_t MAX_ITERATIONS = 100;
    const double BUMP_FACTOR = 1.25;
    const double RESCALE_LIMIT = 1e100;
    const size_t TOP_PAIRS_COUNT = 1;
    std::vector<double> activity;

    mcper_optimizer(std::string path_to_solver, std::string path_to_storage, std::string path_to_dimacs,
                    PGconn *pg_conn, int64_t experiment_id);

    ssize_t fit() override;
};

#endif
