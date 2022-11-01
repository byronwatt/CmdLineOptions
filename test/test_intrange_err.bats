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

@test "err - intrange - start+size" {
  run build/example_with_error_message some_intrange=100+10
  [ $status -eq 0 ]
  assert_output --stdin <<END
option_some_intrange.is_set
option_some_intrange.start_value = 100
option_some_intrange.end_value = 110
END
}

@test "err - intrange - start..badnumber" {
  run build/example_with_error_message some_intrange=0..10f
  [ $status -eq 255 ]
  assert_output --stdin <<END
ParseOptionsOrError returned false
error parsing '0..10f'
 for IntRange option 'some_intrange'
 option description: testing some_intrange
range formats are:
   start..end e.g. 0xd00380..0xd00388
   start+count e.g. 0xd00380+2 (that's 0xd00380,0xd00384)
error parsing "some_intrange=0..10f"
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
