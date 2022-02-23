#include <iostream>
#include <vector>
#include <random>

#include <boost/process.hpp>

namespace bp = boost::process;

static std::mt19937 random_generator;
static size_t n;
static size_t population_size = 10;
static size_t offspring_size = 20;
static size_t mutations_count = 10;
static size_t generations = 1000;

struct cnf {
    std::string header;
    std::vector<std::string> clauses;

    explicit cnf(const std::string &path_to_dimacs) {
        std::ifstream input_stream(path_to_dimacs);
        std::string line;
        while (std::getline(input_stream, line)) {
            if (line[0] == 'c') {
                continue;
            } else if (line.find("p cnf") == 0) {
                std::stringstream line_stream(line);
                std::string dummy;
                line_stream >> dummy >> dummy >> n;
                header = line;
            } else {
                clauses.push_back(line);
            }
        }
    }

    void send_cnf(bp::opstream &in, const std::vector<double> &activity) const {
        in << header << '\n';
        in << "activity " << std::scientific;
        for (const auto &a : activity) {
            in << a << ' ';
        }
        in << '\n';
        for (const auto &s : clauses) {
            in << s << '\n';
        }
        in.close();
        in.pipe().close();
    }
};

struct sample {
    std::vector<double> activity;
    double fitness = NAN;

    sample() : activity(n) {
        std::normal_distribution<double> distribution(0, 1);
        std::generate(activity.begin(), activity.end(), [&distribution] () {
            return std::abs(distribution(random_generator));
        });
    }

    sample(const sample &a, const sample &b) : activity(n) {
        std::uniform_int_distribution<size_t> cut_pos_distribution(0, n - 1);
        size_t cut_pos = cut_pos_distribution(random_generator);
        for (size_t i = 0; i < n; ++i) {
            if (i < cut_pos) {
                activity[i] = a.activity[i];
            } else {
                activity[i] = b.activity[i];
            }
        }
    }

    void evaluate(const cnf &cnf) {
        bp::opstream in;

        std::ostringstream proof_file_path;
//        proof_file_path << static_cast<const void*>(this);
        proof_file_path << "proof.dimacs";

        boost::process::child c("./glucose -certified -certified-output=" + proof_file_path.str(), bp::std_in < in, bp::std_out > bp::null);
        cnf.send_cnf(in, activity);
        c.wait();

        std::ifstream proof_stream(proof_file_path.str());
        size_t proof_size = 0;
        std::string line;
        while (std::getline(proof_stream, line)) {
            ++proof_size;
        }
        fitness = static_cast<double>(proof_size);
    }
};

bool operator<(const sample &a, const sample &b) {
    return a.fitness < b.fitness;
}

bool operator==(const sample &a, const sample &b) {
    return a.fitness == b.fitness;
}

std::vector<sample> create_population() {
    std::vector<sample> population(population_size);
    return population;
}

sample mutate(sample &s) {
    std::bernoulli_distribution distribution(0.25);
    std::normal_distribution<double> normal_distribution(0, 2);
    for (double &a : s.activity) {
        if (distribution(random_generator)) {
            a += normal_distribution(random_generator);
            a = std::abs(a);
        }
    }
    return s;
}

int main() {
    cnf cnf("../../sat/dubois/dubois100.cnf");

    std::vector<sample> population = create_population();
    for (size_t i = 0; i < generations; ++i) {
        std::cout << "=== Generation " << i << " ===\n";
        std::vector<sample> offspring;
        offspring.reserve(offspring_size);
        std::uniform_int_distribution<size_t> population_distribution(0, population_size - 1);
        std::uniform_int_distribution<size_t> offspring_distribution(0, offspring_size - 1);

        std::cout << "Generating offspring...\n";
        for (size_t j = 0; j < offspring_size; ++j) {
            size_t parent_a = population_distribution(random_generator);
            size_t parent_b = population_distribution(random_generator);
            offspring.emplace_back(population[parent_a], population[parent_b]);
        }

        std::cout << "Mutating...\n";
        for (size_t j = 0; j < mutations_count; ++j) {
            mutate(offspring[offspring_distribution(random_generator)]);
        }

        std::cout << "Evaluating...\n";
        for (size_t j = 0; j < offspring_size; ++j) {
            offspring[j].evaluate(cnf);
        }

        std::sort(offspring.begin(), offspring.end());

        std::cout << "Top results: ";
        for (size_t j = 0; j < offspring_size; ++j) {
            std::cout << offspring[j].fitness << ' ';
        }
        std::cout << '\n';
        population.clear();
        for (size_t j = 0; j < population_size; ++j) {
            population.push_back(offspring[j]);
        }
        offspring.clear();
    }
}