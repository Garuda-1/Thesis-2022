#include <iostream>

#include "experiment.h"
#include "../optimizers/cmaes/cmaes_optimizer.h"
#include "../optimizers/commons/commons.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <libpq-fe.h>

int64_t create_experiment(const std::string& name, pg_conn *pg_conn) {
    std::string query = "INSERT INTO experiments (name) VALUES ('" + name + "') RETURNING id";

    PGresult *result;
    result = PQexec(pg_conn, query.c_str());
    int64_t experiment_id = atoll(PQgetvalue(result, 0, 0));
    PQclear(result);

    return experiment_id;
}

void finish_experiment(int64_t experiment_id, pg_conn *pg_conn) {
    std::string query = "UPDATE experiments SET end_time = NOW() WHERE id = " + std::to_string(experiment_id);

    PGresult *result;
    result = PQexec(pg_conn, query.c_str());
    PQclear(result);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Usage: runner <path_to_config>";
    }

    std::string config(argv[1]);
    
    boost::property_tree::ptree pt;
    boost::property_tree::read_json(config, pt);

    for (const auto &experiment : pt.get_child("experiments")) {
        auto name = experiment.second.get_child("name").get_value<std::string>();
        auto path_to_solver = experiment.second.get_child("path_to_solver").get_value<std::string>();
        auto path_to_dimacs = experiment.second.get_child("path_to_dimacs").get_value<std::string>();
        auto optimizer = experiment.second.get_child("optimizer").get_value<std::string>();
        std::string path_to_storage = boost::filesystem::temp_directory_path().string();
        int64_t experiment_id;

        PGconn *pg_conn = PQconnectdb("host=postgres port=5432 dbname=postgres user=postgres password=postgres");

        std::cout << "Starting experiment '" << name << "'." << std::endl;
        experiment_id = create_experiment(name, pg_conn);
        std::cout << "Experiment id: " << experiment_id << std::endl;

        cmaes_optimizer opt(path_to_solver, path_to_dimacs, path_to_storage, 1, 0.2, -1, pg_conn, experiment_id);
        opt.fit();

        finish_experiment(experiment_id, pg_conn);
        PQfinish(pg_conn);
    }

    
}