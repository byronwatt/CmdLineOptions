cmake_minimum_required (VERSION 2.8.11)

project (example)

add_executable (example example/example.cpp example/option_test.cpp src/cmd_line_options.cpp)


target_include_directories (example PUBLIC inc)