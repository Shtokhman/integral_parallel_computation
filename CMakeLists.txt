cmake_minimum_required(VERSION 3.0)

project(integral_parallel_computation)

set(CXX_CMAKE_STANDARD 11)

set(CMAKE_BUILD_TYPE Release)

find_package(Threads REQUIRED)

# for debug information
# add_compile_options("-DPRINT_INTERMEDIATE_STEPS")


add_executable(main main.cpp measure_time.h read_conf.cpp read_conf.h integration.cpp integration.h)

target_link_libraries(main Threads::Threads)