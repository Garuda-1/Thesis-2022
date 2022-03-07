#include <iostream>

#include "experiment.h"
#include "../optimizers/cmaes/cmaes_optimizer.h"
#include "../optimizers/commons/commons.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
//#include <libpq/libpq.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Usage: runner <path_to_config>";
    }

    std::string config(argv[1]);
    
    boost::property_tree::ptree pt;
    boost::property_tree::read_json(config, pt);

    for (const auto &experiment : pt.get_child("experiments")) {
        std::cout << experiment.second.get_child("path_to_dimacs").get_value<std::string>();
    }

    
}