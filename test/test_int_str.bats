#!/usr/bin/env bats

load "libs/bats-support/load"
load "libs/bats-assert/load"

# doesn't fail,... just silently returns the bottom 32 bits !!!
@test "str int - bigger than max int" {
  run build/example_as_string some_int=2271560481
  [ $status -eq 0 ]

  assert_output --stdin <<END
option_some_int.is_set
option_some_int.value = -2023406815
END
}

# doesn't fail,... just silently returns the bottom 32 bits !!!
@test "str int - much bigger than max int" {
  run build/example_as_string some_int=0xff12345678
  [ $status -eq 0 ]

  assert_output --stdin <<END
option_some_int.is_set
option_some_int.value = 305419896
END
}

@test "str int - bad int" {
  run build/example_as_string some_int=10f
  [ $status -eq 255 ]
  assert_output --partial "error parsing 'some_int=10f'"
}

@test "str int - leading zeros does not convert to octal" {
  run build/example_as_string some_int=010
  [ $status -eq 0 ]
  assert_output --stdin <<END
option_some_int.is_set
option_some_int.value = 10
END
}

@test "str int - from environment variable" {
  export PROJECT_NAME_some_int=123
  run build/example_as_string
  [ $status -eq 0 ]
  assert_output --stdin <<END
setting some_int to "123" (from environment variable PROJECT_NAME_some_int)
option_some_int.is_set
option_some_int.value = 123
END
}
