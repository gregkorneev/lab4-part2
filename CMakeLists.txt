cmake_minimum_required(VERSION 3.20)
project(Task22_Frac CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
  add_compile_options(/W4)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_executable(task22_frac src/task22_frac_simple.cpp)

# Бинарник будет в build/bin/
set_target_properties(task22_frac
  PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)