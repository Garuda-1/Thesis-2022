#include <iostream>
#include <vector>
#include <random>

#include <boost/process.hpp>

namespace bp = boost::process;

static std::mt19937 random_generator;
static size_t n;
static size_t population_size;

struct sample {
    std::vector<double> activity;
    double fitness = NAN;

    sample() : activity(n) {
        std::normal_distribution<double> distribution(0, 1);
        std::generate(activity.begin(), activity.end(), [&distribution] () {
            return std::abs(distribution(random_generator));
        });
    }

    sample(const sample &a, const sample &b, size_t cut_pos) : activity(n) {
        for (size_t i = 0; i < n; ++i) {
            if (i < cut_pos) {
                activity[i] = a.activity[i];
            } else {
                activity[i] = b.activity[i];
            }
        }
    }
};

std::vector<sample> create_population() {
    std::vector<sample> population(population_size);
    return population;
}

std::pair<sample, sample> crossover(const sample &a, const sample &b) {
    std::uniform_int_distribution<size_t> distribution(0, n);
    size_t cut_pos = distribution(random_generator);
    return std::make_pair(sample(a, b, cut_pos), sample(b, a, cut_pos));
}

sample mutate(sample &s) {
    std::bernoulli_distribution distribution(0.1);
    std::normal_distribution<double> normal_distribution(0, 1);
    for (double &a : s.activity) {
        if (distribution(random_generator)) {
            a += normal_distribution(random_generator);
        }
    }
    return s;
}

int main() {
    bp::opstream in;
    bp::ipstream out;

    boost::process::child c("./glucose -certified -certified-output=proof.dimacs", bp::std_in < in, bp::std_out > stdout);

    in << "p cnf 3 3\n" << "1 2 3 0\n" << "-1 -2 0\n" << "-2 -3 0\n" << std::endl;
    in.close();
    c.wait();
}

// main -> create_population = (sample)    [generation_size]
//      \> evolve
// evolve -> crossover      [offspring_size]
//        \> mutate         [count_of_mutations]
//        \> evaluate
//        \> elect          [generation_size]