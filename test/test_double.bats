#!/usr/bin/env bats

load "libs/bats-support/load"
load "libs/bats-assert/load"

@test "double with normal value" {
  run build/example some_double=0.0012345
  [ $status -eq 0 ]
  
  assert_output --stdin <<END
option_some_double.is_set
option_some_double.value = 0.0012345
END
}

@test "bad double" {
  run build/example some_double=0.123f
  [ $status -eq 255 ]
  assert_output --partial "error parsing 'some_double=0.123f'"
}

@test "double from environment variable" {
  export PROJECT_NAME_some_double=123
  run build/example
  [ $status -eq 0 ]
  assert_output --stdin <<END
setting some_double to "123" (from environment variable PROJECT_NAME_some_double)
option_some_double.is_set
option_some_double.value = 123
END
}
