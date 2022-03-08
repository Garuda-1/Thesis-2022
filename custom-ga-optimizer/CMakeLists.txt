cmake_minimum_required(VERSION 3.16.3)
project(Optimizer)

set(CMAKE_CXX_STANDARD 14)

#find_package(Boost COMPONENTS process)

include_directories(.)

add_executable(optimizer
        src/main.cpp)

target_link_libraries(optimizer Boost::boost)
