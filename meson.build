project('cmdlineoptions', 'cpp')

coverage = get_option('b_coverage')
if coverage == true
  message( 'b_coverage not true ???' )
  add_global_arguments('-fprofile-abs-path', '-fno-exceptions', '-fno-rtti', language: 'cpp' )
else
  message( 'b_coverage not true ???' )
endif

cmdlineoptions_dep = declare_dependency(
    include_directories : 'inc',
    sources : 'src/cmd_line_options.cpp'
)

executable('example', 
  'example/example.cpp',
  'example/option_test.cpp',
   dependencies: cmdlineoptions_dep)

executable('example_as_string',
  'example/example_as_string.cpp',
  'example/option_test.cpp',
   dependencies: cmdlineoptions_dep)

executable('example_with_error_message', 
  'example/example_with_error_message.cpp',
  'example/option_test.cpp',
   dependencies: cmdlineoptions_dep)
