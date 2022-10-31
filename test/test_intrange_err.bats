#!/usr/bin/env bats

load "libs/bats-support/load"
load "libs/bats-assert/load"

@test "err - intrange - 0..10" {
  run build/example_with_error_message some_intrange=-10..10
  [ $status -eq 0 ]

  assert_output --stdin <<END
option_some_intrange.is_set
option_some_intrange.start_value = -10
option_some_intrange.end_value = 10
END
}


@test "err - intrange - from environment variable" {
  export PROJECT_NAME_some_intrange=0..10
  run build/example_with_error_message
  [ $status -eq 0 ]
  assert_output --stdin <<END
setting some_intrange to "0..10" (from environment variable PROJECT_NAME_some_intrange)
option_some_intrange.is_set
option_some_intrange.start_value = 0
option_some_intrange.end_value = 10
END
}
