cmake_minimum_required(VERSION 3.15)
project(TSP_GeneticAlgorithm VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native")
elseif(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    set(CMAKE_CXX_FLAGS_RELEASE "/O2")
endif()

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE TSP_SOURCES 
    "${PROJECT_SOURCE_DIR}/src/tsp/*.cpp"
)

file(GLOB_RECURSE GA_SOURCES 
    "${PROJECT_SOURCE_DIR}/src/ga/*.cpp"
)

file(GLOB_RECURSE PARALLEL_SOURCES 
    "${PROJECT_SOURCE_DIR}/src/parallel/*.cpp"
)

add_executable(tsp_ga 
    ${TSP_SOURCES}
    ${GA_SOURCES}
    ${PARALLEL_SOURCES}
    main.cpp
)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
