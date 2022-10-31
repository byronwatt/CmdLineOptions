#!/usr/bin/env bats

load "libs/bats-support/load"
load "libs/bats-assert/load"

@test "err - string - " {
  run build/example_with_error_message some_string="hello there"
  [ $status -eq 0 ]
  
  assert_output --stdin <<END
option_some_string.is_set
option_some_string.value = "hello there"
END
}

@test "err - string - from environment variable" {
  export PROJECT_NAME_some_string="hello there"
  run build/example_with_error_message
  [ $status -eq 0 ]
  assert_output --stdin <<END
setting some_string to "hello there" (from environment variable PROJECT_NAME_some_string)
option_some_string.is_set
option_some_string.value = "hello there"
END
}

# note environment variables don't work for lists,... they put the entire string into the first element.
