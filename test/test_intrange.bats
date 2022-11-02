#!/usr/bin/env bats

load "libs/bats-support/load"
load "libs/bats-assert/load"

@test "intrange - 0..10" {
  run build/example some_intrange=-10..10
  [ $status -eq 0 ]

  assert_output --stdin <<END
option_some_intrange.is_set
option_some_intrange.start_value = -10
option_some_intrange.end_value = 10
END
}

@test "intrange - start+size" {
  run build/example some_intrange=100+10
  [ $status -eq 0 ]
  assert_output --stdin <<END
option_some_intrange.is_set
option_some_intrange.start_value = 100
option_some_intrange.end_value = 110
END
}

@test "intrange - start..badnumber" {
  run build/example some_intrange=0..10f
  [ $status -eq 255 ]
  assert_output --partial <<END
error parsing 'some_intrange=0..10f'
END
}

@test "intrange - start-stop" {
  run build/example some_intrange=0-10
  [ $status -eq 255 ]
  assert_output --partial <<END
error parsing 'some_intrange=0-10'
END
}

@test "intrange - start.stop" {
  run build/example some_intrange=0.10
  [ $status -eq 255 ]
  assert_output --partial <<END
error parsing 'some_intrange=0.10'
END
}

@test "intrange - from environment variable" {
  export PROJECT_NAME_some_intrange=0..10
  run build/example
  [ $status -eq 0 ]
  assert_output --stdin <<END
setting some_intrange to "0..10" (from environment variable PROJECT_NAME_some_intrange)
option_some_intrange.is_set
option_some_intrange.start_value = 0
option_some_intrange.end_value = 10
END
}
