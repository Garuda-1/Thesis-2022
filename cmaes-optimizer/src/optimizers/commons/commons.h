#ifndef OPTIMIZER_COMMONS_H
#define OPTIMIZER_COMMONS_H

#include <string>
#include <iostream>
#include <algorithm>
#include <utility>

#include <boost/process.hpp>
#include <libpq-fe.h>

namespace bp = boost::process;

namespace common {

inline void log(const std::string& message) {
  std::cout << "x " << message << '\n';
}

inline void log() {
  std::cout << "x \n";
}

struct hash_pair {
  template <class T1, class T2>
  size_t operator()(const std::pair<T1, T2>& p) const {
    return std::hash<T1>{}(p.first) ^ std::hash<T1>{}(p.second);
  }
};

inline void record_run_to_db(PGconn* pg_conn, int64_t experiment_id, int64_t value) {
  PGresult* result;
  std::string query = "INSERT INTO runs (experiment_id, proof_size) VALUES (" + std::to_string(experiment_id) + ", " +
                      std::to_string(value) + ")";
  result = PQexec(pg_conn, query.c_str());
  ExecStatusType requestStatus = PQresultStatus(result);
  if (requestStatus == PGRES_COMMAND_OK) {
    common::log("Result recorded");
  } else {
    common::log("Failed to record_run_to_db result, status: " + std::to_string(requestStatus));
  }
  PQclear(result);
}

struct optimizer_options {
  bool record_result_to_db = false;
  bool rnd_init = false;
  bool cleanup = false;
  bool print_solver_output = false;
  int32_t freq_lbd_lim = INT_MAX;
};

struct solver_output {
  ssize_t proof_size;
  std::unordered_map<std::pair<int64_t, int64_t>, size_t, hash_pair> trail_frequencies;
  std::unordered_map<std::pair<int64_t, int64_t>, size_t, hash_pair> conflict_frequencies;
};

}  // namespace common

#endif
