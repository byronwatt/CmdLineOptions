cmake_minimum_required (VERSION 2.8.11)

project (example)

add_executable (example src/cmd_line_options.cpp example/example.cpp example/option_test.cpp)

add_compile_options(--coverage)

add_link_options(--coverage)

target_include_directories (example PUBLIC inc)