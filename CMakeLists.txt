cmake_minimum_required(VERSION 3.14)
project(lab4_part2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_library(intcore
    src/integration.hpp
    src/integration.cpp
)

add_executable(Lab4
    src/main.cpp
)
target_link_libraries(Lab4 PRIVATE intcore)

add_executable(task15_cosh
    src/task15_cosh.cpp
)
target_link_libraries(task15_cosh PRIVATE intcore)
