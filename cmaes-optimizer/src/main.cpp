#include <iostream>
#include <vector>
#include <random>

#include <boost/process.hpp>
#include <libcmaes/cmaes.h>

namespace bp = boost::process;

static std::mt19937 random_generator;

static size_t n = 932;
static std::string path_to_dimacs = "/Thesis/sat/benchmarks_part/mp1-tri_ali_s11_c35_abio_UNS.cnf";
static std::string path_to_glucose = "glucose";
static double best_fitness = -1;
static size_t iteration = 0;

struct cnf {
    std::string header;
    std::vector<std::string> body;

    explicit cnf() {
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
                body.push_back(line);
            }
        }
        std::cout << "Benchmark read complete" << std::endl;
    }

    void send_cnf(bp::opstream &in, const std::vector<double> &activity) const {
        in << header << '\n';
        in << "activity " << std::scientific;
        for (const auto &a : activity) {
            in << a << ' ';
        }
        in << '\n';
        for (const auto &s : body) {
            in << s << '\n';
        }
        in.close();
        in.pipe().close();
    }
};

static cnf test_cnf;

struct sample {
    std::vector<double> activity;
    double fitness = NAN;

    explicit sample(const double *a) : activity(n) {
        activity.assign(a, a + n);
    }

    void evaluate(const cnf &cnf) {
        bp::opstream in;

        std::ostringstream proof_file_path;
        proof_file_path << "proof.dimacs";

        bp::child c(path_to_glucose + " -certified -certified-output=" + proof_file_path.str(), bp::std_in < in, bp::std_out > bp::null);
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

libcmaes::FitFunc unsat_proof_size = [](const double *a, const size_t n) {
    sample sample(a);
    sample.evaluate(test_cnf);
    if (best_fitness < 0 || sample.fitness < best_fitness) {
        best_fitness = sample.fitness;
    }
    std::cout << static_cast<uint64_t>(best_fitness) << " | " << iteration++ << std::endl;
    return sample.fitness;
};

int main() {
    std::cout << "Starting optimization..." << std::endl;
    std::vector<double> x0(n, 1);
    double sigma = 0.2;
    ssize_t lambda = -1;
    libcmaes::CMAParameters<> cmaparams(x0, sigma, lambda);
    libcmaes::CMASolutions cmasols = libcmaes::cmaes<>(unsat_proof_size, cmaparams);
    std::cout << "best solution: " << cmasols << std::endl;
    std::cout << "optimization took " << cmasols.elapsed_time() / 1000.0 << " seconds\n";
    return cmasols.run_status();
}