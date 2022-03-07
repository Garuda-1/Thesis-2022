cmake_minimum_required(VERSION 3.16)
project(Optimizer)

set(CMAKE_CXX_STANDARD 17)

# Search for packages
find_package(libcmaes REQUIRED)
find_package(Boost COMPONENTS filesystem REQUIRED)
find_package(PostgreSQL 9.6.2 REQUIRED)
find_package(ZLIB)

include_directories(${Boost_LIBRARIES})
include_directories(src/optimizers)
include_directories(src/runner)
include_directories(src/glucose)

# Build standalone optimizers
add_executable(optimizer
        src/optimizers/cmaes/main.cpp
        src/optimizers/cmaes/cmaes_optimizer.h
        src/optimizers/cmaes/cmaes_optimizer.cpp
        src/optimizers/commons/commons.h
        src/optimizers/commons/commons.cpp)

target_link_libraries(optimizer
        libcmaes::cmaes
        ${Boost_LIBRARIES})

# Build experiments runner
add_executable(runner
        src/runner/experiment.h
        src/runner/runner.cpp
        src/optimizers/cmaes/cmaes_optimizer.h
        src/optimizers/cmaes/cmaes_optimizer.cpp
        src/optimizers/commons/commons.h
        src/optimizers/commons/commons.cpp)

target_link_libraries(runner
        libcmaes::cmaes
        ${Boost_LIBRARIES}       )
#        /*PostgreSQL::PostgreSQL*/)

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
