cmake_minimum_required(VERSION 3.13)
project(microtunnel C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
file(MAKE_DIRECTORY tmp)

add_compile_options(-fstack-protector-all -Wall -Wpedantic -g)

# ------ main ----------------------------------------------------------------
#
add_executable(main
  src/main.cpp
)

target_include_directories(main PRIVATE src)
