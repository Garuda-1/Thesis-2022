cmake_minimum_required(VERSION 3.16)
project(Optimizer)

set(CMAKE_CXX_STANDARD 17)

# Search for packages
find_package(libcmaes REQUIRED)
#find_package(paradiseo REQUIRED)
find_package(Boost COMPONENTS filesystem REQUIRED)
find_package(PostgreSQL 9.6.2 REQUIRED)
find_package(ZLIB)

add_subdirectory(src/third-party/openga)

include_directories(${Boost_LIBRARIES})
#include_directories(${PARADISEO_INCLUDE_DIR})
include_directories(src/optimizers)
include_directories(src/runner)
include_directories(src/glucose)

## Build standalone optimizers
#add_executable(optimizer
#        src/optimizers/cmaes/main.cpp
#        src/optimizers/cmaes/cmaes_optimizer.h
#        src/optimizers/cmaes/cmaes_optimizer.cpp
#        src/optimizers/commons/cnf.h
#        src/optimizers/commons/cnf.cpp
#        src/optimizers/commons/sample.h
#        src/optimizers/commons/sample.cpp
#        src/optimizers/commons/solver.h
#        src/optimizers/commons/solver.cpp
#        src/optimizers/commons/commons.h)
#
#target_link_libraries(optimizer
#        libcmaes::cmaes
#        ${Boost_LIBRARIES}
#        PostgreSQL::PostgreSQL)

# Build experiments runner
add_executable(runner
        src/runner/experiment.h
        src/runner/runner.cpp
        src/optimizers/cmaes/cmaes_optimizer.h
        src/optimizers/cmaes/cmaes_optimizer.cpp
        src/optimizers/gaer/gaer_optimizer.h
        src/optimizers/gaer/gaer_optimizer.cpp
        src/optimizers/mcper/mcper_optimizer.h
        src/optimizers/mcper/mcper_optimizer.cpp
        src/optimizers/commons/cnf.h
        src/optimizers/commons/cnf.cpp
        src/optimizers/commons/cnf_er.h
        src/optimizers/commons/cnf_er.cpp
        src/optimizers/commons/optimizer.h
        src/optimizers/commons/optimizer.cpp
        src/optimizers/commons/sample.h
        src/optimizers/commons/sample.cpp
        src/optimizers/commons/solver.h
        src/optimizers/commons/solver.cpp
        src/optimizers/commons/commons.h)

target_link_libraries(runner
        libcmaes::cmaes
        OpenGA
#        ${PARADISEO_LIBRARIES}
        ${Boost_LIBRARIES}
        PostgreSQL::PostgreSQL)

# Build SAT solver
add_executable(glucose
        src/glucose/core/BoundedQueue.h
        src/glucose/core/Constants.h
        src/glucose/core/Dimacs.h
        src/glucose/core/Main.cc
        src/glucose/core/Solver.cc
        src/glucose/core/Solver.h
        src/glucose/core/SolverTypes.h
        src/glucose/mtl/Alg.h
        src/glucose/mtl/Alloc.h
        src/glucose/mtl/Heap.h
        src/glucose/mtl/IntTypes.h
        src/glucose/mtl/Map.h
        src/glucose/mtl/Queue.h
        src/glucose/mtl/Sort.h
        src/glucose/mtl/Vec.h
        src/glucose/mtl/XAlloc.h
        src/glucose/utils/Options.cc
        src/glucose/utils/Options.h
        src/glucose/utils/ParseUtils.h
        src/glucose/utils/System.cc
        src/glucose/utils/System.h)

target_link_libraries(glucose ZLIB::ZLIB)
