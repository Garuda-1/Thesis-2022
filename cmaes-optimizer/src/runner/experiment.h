#ifndef OPTIMIZER_EXPERIMENT_H
#define OPTIMIZER_EXPERIMENT_H

#include <string>

struct experiment {
    std::string name;
    std::string path_to_solver;
    std::string path_to_dimacs;
};

#endif
