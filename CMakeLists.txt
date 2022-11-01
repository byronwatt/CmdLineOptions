cmake_minimum_required (VERSION 2.8.11)

project (example)

add_executable (example example/example.cpp example/option_test.cpp src/cmd_line_options.cpp )

target_compile_options(example PUBLIC --coverage)

target_link_options(example PUBLIC --coverage)

target_include_directories (example PUBLIC inc)



add_executable (example_as_string example/example_as_string.cpp example/option_test.cpp src/cmd_line_options.cpp )

target_compile_options(example_as_string PUBLIC --coverage)

target_link_options(example_as_string PUBLIC --coverage)

target_include_directories (example_as_string PUBLIC inc)



add_executable (example_with_error_message example/example_with_error_message.cpp example/option_test.cpp src/cmd_line_options.cpp )

target_compile_options(example_with_error_message PUBLIC --coverage)

target_link_options(example_with_error_message PUBLIC --coverage)

target_include_directories (example_with_error_message PUBLIC inc)