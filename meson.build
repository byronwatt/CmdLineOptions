project('cmdlineoptions', 'cpp')

inc = include_directories(
  'inc'
)

sources = [
  'example/example.cpp',
  'example/option_test.cpp',
  'src/cmd_line_options.cpp',
]

executable('example', 
   sources: [sources],
   include_directories : [inc])
   

executable('example_as_string',
  'example/example_as_string.cpp',
  'example/option_test.cpp',
  'src/cmd_line_options.cpp',
   include_directories : [inc])

   

executable('example_with_error_message', 
  'example/example_with_error_message.cpp',
  'example/option_test.cpp',
  'src/cmd_line_options.cpp',
   include_directories : [inc])
