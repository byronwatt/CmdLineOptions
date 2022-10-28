cmake_minimum_required (VERSION 2.8.11)

project (CmdLineOptions)

add_executable (CmdLineOptions example/example.cpp example/option_test.cpp src/cmd_line_options.cpp)


target_include_directories (CmdLineOptions PUBLIC inc)