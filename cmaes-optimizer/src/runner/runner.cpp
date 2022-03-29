#include <iostream>

#define OPENGA_EXTERN_LOCAL_VARS

#include "experiment.h"
#include "../optimizers/cmaes/cmaes_optimizer.h"
#include "../optimizers/mcper/mcper_optimizer.h"
#include "../optimizers/gaa/gaa_optimizer.h"
#include "../optimizers/gaer/gaer_optimizer.h"

#include <memory>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <libpq-fe.h>

int64_t create_experiment(const std::string &name, pg_conn *pg_conn) {
    std::string query = "INSERT INTO experiments (name) VALUES ('" + name + "') RETURNING id";

    PGresult *result;
    result = PQexec(pg_conn, query.c_str());
    std::string result_string = std::string(PQgetvalue(result, 0, 0));
    int64_t experiment_id = std::stoll(result_string);
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

    for (const auto &experiment: pt.get_child("experiments")) {
        auto name = experiment.second.get_child("name").get_value<std::string>();
        auto path_to_solver = experiment.second.get_child("path_to_solver").get_value<std::string>();
        auto path_to_dimacs = experiment.second.get_child("path_to_dimacs").get_value<std::string>();
        auto optimizer_name = experiment.second.get_child("optimizer_name").get_value<std::string>();
        int64_t experiment_id;

        PGconn *pg_conn = PQconnectdb("host=postgres port=5432 dbname=postgres user=postgres password=postgres");

        std::cout << "Starting experiment '" << name << "'." << std::endl;
        experiment_id = create_experiment(name, pg_conn);
        std::cout << "Experiment id: " << experiment_id << std::endl;

        std::string path_to_storage =
                boost::filesystem::temp_directory_path().string() + "/experiments/" + std::to_string(experiment_id);
        boost::filesystem::create_directories(path_to_storage);
        std::cout << "Temp files directory: " << path_to_storage << std::endl;

        std::unique_ptr<optimizer> optimizer;

        if (optimizer_name == "cmaes") {
            optimizer = std::make_unique<cmaes_optimizer>(path_to_solver, path_to_dimacs, path_to_storage, 1, 0.2,
                                                          -1, pg_conn, experiment_id);
        } else if (optimizer_name == "mcper") {
            optimizer = std::make_unique<mcper_optimizer>(path_to_solver, path_to_storage, path_to_dimacs, pg_conn, experiment_id);
        } else if (optimizer_name == "gaer") {
            optimizer = std::make_unique<gaer_optimizer>(path_to_solver, path_to_storage, path_to_dimacs, pg_conn, experiment_id);
        } else if (optimizer_name == "gaa") {
            optimizer = std::make_unique<gaa_optimizer>(path_to_solver, path_to_storage, path_to_dimacs, pg_conn, experiment_id);
        } else {
            throw std::runtime_error("Unknown optimizer: " + optimizer_name);
        }

        optimizer->fit();

        finish_experiment(experiment_id, pg_conn);
        PQfinish(pg_conn);
    }
}