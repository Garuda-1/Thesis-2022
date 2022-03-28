#ifndef OPTIMIZER_GAER_OPTIMIZER_H
#define OPTIMIZER_GAER_OPTIMIZER_H

#include "commons/commons.h"
#include "commons/optimizer.h"
#include "commons/cnf_er.h"

#include <openGA.hpp>

struct gaer_optimizer : optimizer {

    std::mt19937 gen;

    gaer_optimizer(std::string path_to_solver, std::string path_to_storage, std::string path_to_dimacs,
                   PGconn *pg_conn, int64_t experiment_id);

    void init_genes(common::cnf_er &cnf, const std::function<double(void)> &rnd);

    bool eval_solution(const common::cnf_er &cnf, common::sample &sample);

    common::cnf_er mutate(const common::cnf_er &base, const std::function<double(void)> &rnd, double shrink_scale);

    common::cnf_er crossover(const common::cnf_er &a, const common::cnf_er &b, const std::function<double(void)> &rnd);

    double calculate(const EA::Genetic<common::cnf_er, common::sample>::thisChromosomeType &x);

    ssize_t fit() override;
};


#endif
