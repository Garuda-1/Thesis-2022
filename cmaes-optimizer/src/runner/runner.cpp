#include <iostream>

#include "../optimizers/cmaes/cmaes_optimizer.h"
#include "../optimizers/commons/commons.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Usage: runner <path_to_config>";
    }

    std::string config(argv[1]);
    
}