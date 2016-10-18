cmake_minimum_required(VERSION 3.6.2 FATAL_ERROR)
project(time_series_indexing)
include_directories(include)
file(MAKE_DIRECTORY "bin/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin/")

# Serialization test main
set(SOURCES src/serialize.cpp src/test_serialize.cpp)
add_executable(outCmake ${SOURCES})
target_compile_features(outCmake PRIVATE cxx_range_for)

# Querying test main
set(SOURCES src/querying.cpp src/test_querying.cpp src/serialize.cpp)
add_executable(test_querying ${SOURCES})
target_compile_features(test_querying PRIVATE cxx_range_for)

# Random queries maker
set(SOURCES src/queries_maker.cpp)
add_executable(queries_maker ${SOURCES})
target_compile_features(queries_maker PRIVATE cxx_range_for)
