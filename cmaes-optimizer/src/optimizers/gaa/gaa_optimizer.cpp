#include "gaa_optimizer.h"

typedef EA::Genetic<common::cnf_a, common::sample> ga_type;
typedef EA::GenerationType<common::cnf_a, common::sample> generation_type;

gaa_optimizer::gaa_optimizer(std::string path_to_solver, std::string path_to_storage, std::string path_to_dimacs,
                             PGconn *pg_conn, int64_t experiment_id) :
        optimizer(std::move(path_to_solver), std::move(path_to_storage), common::cnf(std::move(path_to_dimacs)),
                  pg_conn, experiment_id) {
}

void gaa_optimizer::init_genes(common::cnf_a &cnf, const std::function<double(void)> &rnd) {
    cnf = common::cnf_a(this->benchmark);

    cnf.activity.reserve(cnf.var_count);
    for (size_t i = 0; i < cnf.var_count; ++i) {
        cnf.activity.push_back(rnd());
    }
}

bool gaa_optimizer::eval_solution(const common::cnf_a &cnf, common::sample &sample) {
    common::optimizer_options options = {false, false, true, true};
    evaluate_and_record(cnf, sample, options);
    return true;
}

common::cnf_a gaa_optimizer::mutate(const common::cnf_a &cnf,
                                    const std::function<double(void)> &rnd,
                                    double shrink_scale) {
    common::cnf_a mutated = cnf;
    common::log("Shrink scale: " + std::to_string(shrink_scale));
    for (size_t i = 0; i < mutated.var_count; ++i) {
        if (rnd() < 0.1) {
            mutated.activity[i] *= rnd() * 2;
        }
    }
    return mutated;
}

void gaa_optimizer::report_generation(int generation_number,
                                      const EA::GenerationType<common::cnf_a, common::sample> &last_generation,
                                      const common::cnf_a &best_genes) {
    common::record_run_to_db(pg_conn, experiment_id, static_cast<int64_t>(last_generation.best_total_cost));
}

common::cnf_a gaa_optimizer::crossover(const common::cnf_a &a,
                                       const common::cnf_a &b,
                                       const std::function<double(void)> &rnd) {
    common::cnf_a offspring = a;
    auto crossover_point = static_cast<size_t>(rnd() * static_cast<double>(offspring.var_count));
    for (size_t i = 0; i < offspring.var_count; ++i) {
        offspring.activity[i] = (i < crossover_point) ? a.activity[i] : b.activity[i];
    }
    return offspring;
}

double gaa_optimizer::calculate(const EA::Genetic<common::cnf_a, common::sample>::thisChromosomeType &x) {
    return x.middle_costs.solver_output.proof_size;
}

ssize_t gaa_optimizer::fit() {
    ga_type ga;
    ga.problem_mode = EA::GA_MODE::SOGA;
    ga.multi_threading = false;
    ga.verbose = true;
    ga.population = 50;
    ga.generation_max = 200;
    ga.calculate_SO_total_fitness = std::bind(&gaa_optimizer::calculate, this, std::placeholders::_1);
    ga.init_genes = std::bind(&gaa_optimizer::init_genes, this, std::placeholders::_1, std::placeholders::_2);
    ga.eval_solution = std::bind(&gaa_optimizer::eval_solution, this, std::placeholders::_1, std::placeholders::_2);
    ga.mutate = std::bind(&gaa_optimizer::mutate, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    ga.crossover = std::bind(&gaa_optimizer::crossover, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    ga.SO_report_generation = std::bind(&gaa_optimizer::report_generation, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    ga.best_stall_max = 1000;
    ga.average_stall_max = 1000;
    ga.elite_count = 10;
    ga.crossover_fraction = 0.5;
    ga.mutation_rate = 0.05;

    ga.solve();

    return static_cast<ssize_t>(ga.last_generation.best_total_cost);
}


