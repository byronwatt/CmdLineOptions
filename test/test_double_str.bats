#!/usr/bin/env bats

load "libs/bats-support/load"
load "libs/bats-assert/load"

@test "double with normal value" {
  run build/example_as_string some_double=0.0012345
  [ $status -eq 0 ]
  
  assert_output --stdin <<END
option_some_double.is_set
option_some_double.value = 0.0012345
END
}

@test "double with a negative value" {
  run build/example_as_string some_double=-0.0012345
  [ $status -eq 0 ]
  
  assert_output --stdin <<END
option_some_double.is_set
option_some_double.value = -0.0012345
END
}


@test "double with a fraction" {
  echo "note: setting to a fraction also display an explanatory message so that the magic isn't unexpected"
  run build/example_as_string some_double=5/16
  [ $status -eq 0 ]
  
  assert_output --stdin <<END
setting some_double to 5/16 = 0.3125
option_some_double.is_set
option_some_double.value = 0.3125
END
}

@test "bad double" {
  run build/example_as_string some_double=0.123f
  [ $status -eq 255 ]
  assert_output --partial "error parsing 'some_double=0.123f'"
}

@test "double from environment variable" {
  echo "note: setting to an environment variable also displays an expository message."
  echo "environmental side effects could be hard to debug, so need to advertise it somehow."
  export PROJECT_NAME_some_double=123
  run build/example_as_string
  [ $status -eq 0 ]
  assert_output --stdin <<END
setting some_double to "123" (from environment variable PROJECT_NAME_some_double)
option_some_double.is_set
option_some_double.value = 123
END
}
