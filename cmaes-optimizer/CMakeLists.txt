cmake_minimum_required(VERSION 3.16)
project(Optimizer)

set(CMAKE_CXX_STANDARD 14)

add_executable(optimizer src/main.cpp)

find_package(libcmaes REQUIRED)
find_package(Boost REQUIRED)

target_link_libraries(optimizer libcmaes::cmaes ${Boost_LIBRARIES})

message(STATUS ${libcmaes_FOUND})


include_directories(src/glucose)

find_package(ZLIB)

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

#set(libcmaes_DIR /libcmaes/build)
#set(CMAKE_PREFIX_PATH /home/user/.local)
#find_package(libcmaes REQUIRED)

#include_directories(.)

