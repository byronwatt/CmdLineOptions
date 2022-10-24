project('cmdlineoptions', 'cpp')

inc = include_directories(
  'inc'
)

sources = [
  'src/cmd_line_options.cpp',
  'example/example.cpp',
  'example/option_test.cpp',
]

executable('example', 
   sources: [sources],
   include_directories : [inc])
