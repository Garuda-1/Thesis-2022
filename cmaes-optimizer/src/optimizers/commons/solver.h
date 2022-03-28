#ifndef OPTIMIZER_SOLVER_H
#define OPTIMIZER_SOLVER_H

#include "cnf.h"

namespace common {

struct cnf;

struct solver {
    std::string path;

    explicit solver(std::string path) : path(std::move(path)) {}

    [[nodiscard]] ssize_t unsat_proof_size(const cnf &cnf, const std::string &path_to_storage,
                                           const std::optional<std::vector<double>> &activity,
                                           std::string &proof_file_path) const;
};

}

#endif
