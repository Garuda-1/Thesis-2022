#include <iostream>

#include "cmaes/cmaes_optimizer.h"
#include "commons/commons.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cout << "Usage: optimizer_cmaes <path_to_solver> <path_to_dimacs>";
        return 0;
    }

    std::string path_to_solver(argv[1]);
    std::string path_to_dimacs(argv[2]);

    common::log("CMA-ES based UNSAT proof optimizer");
    common::log();

    common::log("Creating an optimizer...");
    std::string path_to_storage = boost::filesystem::temp_directory_path().string();
    common::log("Logs will be stored at " + path_to_storage);
    cmaes_optimizer optimizer(path_to_solver, path_to_dimacs, boost::filesystem::temp_directory_path().string());
    common::log("Optimizer created");
    common::log();

    common::log("Starting optimization...");
    optimizer.fit();
}
