cmake_minimum_required(VERSION 3.6.2 FATAL_ERROR)
project(time_series_indexing)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin/")
set(CMAKE_BUILD_TYPE Release)

set(GCC_SDSL_FLAGS "-lsdsl -ldivsufsort -ldivsufsort64")
add_definitions(${GCC_SDSL_FLAGS})

SET(exec_1 build_index)
ADD_EXECUTABLE(${exec_1} src/Baseline.hpp src/Index1.hpp src/build_index.cpp)
TARGET_LINK_LIBRARIES(${exec_1} sdsl divsufsort divsufsort64)
SET_PROPERTY(TARGET ${exec_1} PROPERTY COMPILE_DEFINITIONS)

SET(exec_2 run_queries)
ADD_EXECUTABLE(${exec_2} src/Baseline.hpp src/Index1.hpp src/run_queries.cpp)
TARGET_LINK_LIBRARIES(${exec_2} sdsl divsufsort divsufsort64)
SET_PROPERTY(TARGET ${exec_2} PROPERTY COMPILE_DEFINITIONS)

SET(exec_3 build_query_sets)
ADD_EXECUTABLE(${exec_3} src/Baseline.hpp src/Index1.hpp src/build_query_sets.cpp)
TARGET_LINK_LIBRARIES(${exec_3} sdsl divsufsort divsufsort64)
SET_PROPERTY(TARGET ${exec_3} PROPERTY COMPILE_DEFINITIONS)
