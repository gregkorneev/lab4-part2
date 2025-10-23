cmake_minimum_required(VERSION 3.10)
project(Lab4)

set(CMAKE_CXX_STANDARD 11)

# Создание исполняемого файла
add_executable(Lab4 src/main.cpp)

# Включение директорий с заголовками
target_include_directories(Lab4 PRIVATE include)

# Флаги компиляции
target_compile_options(Lab4 PRIVATE -Wall -Wextra -O2)  