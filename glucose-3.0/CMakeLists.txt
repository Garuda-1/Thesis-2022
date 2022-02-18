cmake_minimum_required(VERSION 3.16)
project(Glucose)

set(CMAKE_CXX_STANDARD 14)

find_package(ZLIB)

include_directories(.)

add_executable(glucose
        core/BoundedQueue.h
        core/Constants.h
        core/Dimacs.h
        core/Main.cc
        core/Solver.cc
        core/Solver.h
        core/SolverTypes.h
        mtl/Alg.h
        mtl/Alloc.h
        mtl/Heap.h
        mtl/IntTypes.h
        mtl/Map.h
        mtl/Queue.h
        mtl/Sort.h
        mtl/Vec.h
        mtl/XAlloc.h
        utils/Options.cc
        utils/Options.h
        utils/ParseUtils.h
        utils/System.cc
        utils/System.h)

target_link_libraries(glucose ZLIB::ZLIB)