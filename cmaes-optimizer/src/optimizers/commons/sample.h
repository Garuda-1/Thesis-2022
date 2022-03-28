#ifndef OPTIMIZER_SAMPLE_H
#define OPTIMIZER_SAMPLE_H

#include "cnf.h"

#include <vector>

namespace common {

    struct cnf;

    struct sample {
        std::optional<std::vector<double>> activity;
        ssize_t fitness = -1;

        sample() = default;

        sample(const double *a, size_t var_count);

        explicit sample(std::vector<double> a);

        void evaluate(const cnf &cnf, const std::string &path_to_solver, const std::string &path_to_storage,
                      std::string &path_to_proof);
    };

}

#endif
