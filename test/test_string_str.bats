#!/usr/bin/env bats

load "libs/bats-support/load"
load "libs/bats-assert/load"

@test "str - string - ParseString doesn't handle embedded spaces" {
  run build/example_as_string some_string="hello there"
  [ $status -eq 255 ]
  
  assert_output --partial <<END
no match for 'there'
END
}

@test "str - string" {
  run build/example_as_string some_string=hello_there
  [ $status -eq 0 ]
  
  assert_output --partial <<END
option_some_string.is_set
option_some_string.value = "hello there"
END
}

@test "str - string - from environment variable" {
  export PROJECT_NAME_some_string="hello there"
  run build/example_as_string
  [ $status -eq 0 ]
  assert_output --stdin <<END
setting some_string to "hello there" (from environment variable PROJECT_NAME_some_string)
option_some_string.is_set
option_some_string.value = "hello there"
END
}

# note environment variables don't work for lists,... they put the entire string into the first element.
