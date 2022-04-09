#ifndef OPTIMIZER_SOLVER_H
#define OPTIMIZER_SOLVER_H

#include "cnf.h"
#include "commons.h"

#include <random>
#include <sstream>
#include <boost/asio.hpp>

namespace common {

    struct cnf;
    struct optimizer_options;
    struct solver_output;

    struct solver {
        std::string path;

        explicit solver(std::string path) : path(std::move(path)) {}

        [[nodiscard]] common::solver_output run_unsat(const cnf &cnf, const std::string &path_to_storage,
                                                      const std::optional<std::vector<double>> &activity,
                                                      common::optimizer_options &options) const;
    };

}

#endif
