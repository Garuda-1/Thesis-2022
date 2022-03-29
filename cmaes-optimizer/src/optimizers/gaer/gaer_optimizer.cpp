#include "gaer_optimizer.h"

typedef EA::Genetic<common::cnf_er, common::sample> ga_type;
typedef EA::GenerationType<common::cnf_er, common::sample> generation_type;

gaer_optimizer::gaer_optimizer(std::string path_to_solver, std::string path_to_storage, std::string path_to_dimacs,
                               PGconn *pg_conn, int64_t experiment_id) :
       optimizer(std::move(path_to_solver), std::move(path_to_storage), common::cnf(std::move(path_to_dimacs)),
                 pg_conn, experiment_id) {
}

void gaer_optimizer::init_genes(common::cnf_er &cnf, const std::function<double(void)> &rnd) {
    cnf = common::cnf_er(this->benchmark);
    auto new_pairs_count = static_cast<size_t>(rnd() * cnf.var_count/* * 0.1*/);
    common::log("Initializing genes with " + std::to_string(new_pairs_count) + " pairs");

    for (size_t i = 0; i < new_pairs_count; ++i) {
        auto a = (static_cast<int64_t>(rnd() * cnf.var_count) + 1) * (rnd() < 0.5 ? -1 : 1);
        auto b = (static_cast<int64_t>(rnd() * cnf.var_count) + 1) * (rnd() < 0.5 ? -1 : 1);
        if (a > b) {
            std::swap(a, b);
        }
        cnf.er_pairs.insert({a, b});
    }
}

bool gaer_optimizer::eval_solution(const common::cnf_er &cnf, common::sample &sample) {
    std::string proof_file_path;
    evaluate_and_record(cnf, sample, proof_file_path, false);
    return true;
}

common::cnf_er gaer_optimizer::mutate(const common::cnf_er &cnf,
                                      const std::function<double(void)> &rnd,
                                      double shrink_scale) {
    common::cnf_er mutated = cnf;

    auto new_pairs_count = static_cast<size_t>(shrink_scale * mutated.er_pairs.size() * rnd()/* * 0.1*/);
    auto deleted_pairs_count = static_cast<size_t>(shrink_scale * mutated.er_pairs.size() * rnd()/* * 0.1*/);

    for (size_t i = 0; i < new_pairs_count; ++i) {
        auto a = (static_cast<int64_t>(rnd() * mutated.var_count) + 1) * (rnd() < 0.5 ? -1 : 1);
        auto b = (static_cast<int64_t>(rnd() * mutated.var_count) + 1) * (rnd() < 0.5 ? -1 : 1);
        if (a > b) {
            std::swap(a, b);
        }
        mutated.er_pairs.insert({a, b});
    }

    for (size_t i = 0; i < deleted_pairs_count && !mutated.er_pairs.empty(); ++i) {
        std::pair<int64_t, int64_t> sample_pair;
        std::sample(mutated.er_pairs.begin(), mutated.er_pairs.end(), &sample_pair, 1, gen);
        mutated.er_pairs.erase(mutated.er_pairs.find(sample_pair));
    }

    return mutated;
}

void gaer_optimizer::report_generation(int generation_number,
                                       const EA::GenerationType<common::cnf_er, common::sample> &last_generation,
                                       const common::cnf_er &best_genes) {
    common::log("Best genes pairs size: " + std::to_string(best_genes.er_pairs.size()));
    common::record_run_to_db(pg_conn, experiment_id, static_cast<int64_t>(last_generation.best_total_cost));
}

common::cnf_er gaer_optimizer::crossover(const common::cnf_er &a,
                                         const common::cnf_er &b,
                                         const std::function<double(void)> &rnd) {
    common::cnf_er offspring = a;
    offspring.er_pairs.clear();
    double offset = rnd();

    std::vector<std::pair<int64_t, int64_t>> a_sample(a.er_pairs.size() * offset);
    std::sample(a.er_pairs.begin(), a.er_pairs.end(), a_sample.begin(), a_sample.size(), gen);
    offspring.er_pairs.insert(a_sample.begin(), a_sample.end());

    std::vector<std::pair<int64_t, int64_t>> b_sample(b.er_pairs.size() * (1 - offset));
    std::sample(b.er_pairs.begin(), b.er_pairs.end(), b_sample.begin(), b_sample.size(), gen);
    offspring.er_pairs.insert(b_sample.begin(), b_sample.end());

    return offspring;
}

double gaer_optimizer::calculate(const EA::Genetic<common::cnf_er, common::sample>::thisChromosomeType &x) {
    return x.middle_costs.fitness;
}

ssize_t gaer_optimizer::fit() {
    ga_type ga;
    ga.problem_mode = EA::GA_MODE::SOGA;
    ga.multi_threading = false;
    ga.verbose = true;
    ga.population = 50;
    ga.generation_max = 200;
    ga.calculate_SO_total_fitness = std::bind(&gaer_optimizer::calculate, this, std::placeholders::_1);
    ga.init_genes = std::bind(&gaer_optimizer::init_genes, this, std::placeholders::_1, std::placeholders::_2);
    ga.eval_solution = std::bind(&gaer_optimizer::eval_solution, this, std::placeholders::_1, std::placeholders::_2);
    ga.mutate = std::bind(&gaer_optimizer::mutate, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    ga.crossover = std::bind(&gaer_optimizer::crossover, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    ga.SO_report_generation = std::bind(&gaer_optimizer::report_generation, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    ga.best_stall_max = 1000;
    ga.average_stall_max = 1000;
    ga.elite_count = 10;
    ga.crossover_fraction = 0.5;
    ga.mutation_rate = 0.05;

    ga.solve();

    return static_cast<ssize_t>(ga.last_generation.best_total_cost);
}


