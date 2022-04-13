#include "solver.h"

common::solver_output common::solver::run_unsat(const cnf &cnf, const std::string &path_to_storage,
                                                const std::optional<std::vector<double>> &activity,
                                                common::optimizer_options &options) const {
    std::random_device random_device;
    std::mt19937 random_gen(random_device());
    std::uniform_int_distribution distribution(1, 10000);

    bp::opstream in;
    boost::asio::io_service ios;
    std::future<std::string> data;
    std::string command = path + " -verb=0";

    if (options.rnd_init) {
        command += " -rnd-init -rnd-seed=" + std::to_string(distribution(random_gen));
    }
    bp::child c(command, bp::std_in < in, bp::std_out > data, ios);
    cnf.write_cnf_header(in);
    if (activity) {
        cnf.write_cnf_activity(in, *activity);
    }
    cnf.write_cnf_clauses(in);
    in.close();
    in.pipe().close();

    ios.run();
    c.wait();
    data.wait();

    common::solver_output output;
    std::string line;
    std::stringstream child_out(data.get());
    int64_t ft_lc = 0;
    int64_t fc_lc = 0;
    while (std::getline(child_out, line)) {
        std::stringstream line_stream(line);
        std::string line_start;
        line_stream >> line_start;
        if (line_start == "ps") {
            line_stream >> output.proof_size;
        } else if (line_start == "ft") {
            for (int64_t i = 0; i < 2 * cnf.var_count; ++i) {
                size_t f;
                line_stream >> f;
                if (f == 0) {
                    continue;
                }
                int64_t a = ((ft_lc >> 1) + 1) * (-2 * (ft_lc & 1) + 1);
                int64_t b = ((i >> 1) + 1) * (-2 * (i & 1) + 1);
                output.trail_frequencies[{a, b}] = f;
            }
            ++ft_lc;
        } else if (line_start == "fc") {
            for (int64_t i = 0; i < 2 * cnf.var_count; ++i) {
                size_t f;
                line_stream >> f;
                if (f == 0) {
                    continue;
                }
                int64_t a = ((fc_lc >> 1) + 1) * (-2 * (fc_lc & 1) + 1);
                int64_t b = ((i >> 1) + 1) * (-2 * (i & 1) + 1);
                output.conflict_frequencies[{a, b}] = f;
            }
            ++fc_lc;
        }

        if (options.print_solver_output) {
            common::log(line);
        }
    }

    return output;
}