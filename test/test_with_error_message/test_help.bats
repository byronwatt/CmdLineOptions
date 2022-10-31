#!/usr/bin/env bats

load "../libs/bats-support/load"
load "../libs/bats-assert/load"


@test "no arguments no output" {
  echo "running build/example_with_error_message"
  run build/example_with_error_message
  [ $status -eq 0 ]
  assert_output --stdin <<END
END
}

@test "bad arguments displays help message" {
  run build/example_with_error_message asdf
  [ $status -eq 255 ]
  assert_output --stdin <<END
ParseOptionsOrError returned false
no match for option "asdf"

example_with_error_message
  - demonstrates how options can be passed as messages and parsed
  - does not assert on failure,... instead outputs error message to a std::ostream

  some_bool        testing bool option
  some_enum        testing some_enum
  some_int         testing some_int
  some_uint        testing some_uint
  some_int64       testing some_int64
  some_uint64      testing some_uint64
  some_intrange    testing some_intrange
  some_intList:    testing some_intList
  some_stringlist: testing some_stringlist
  some_double      testing some_double
  some_string      testing some_string
  log_level        set SW APPLIB logging level (0=DEBUG,1=INFO,2=WARNING,3=ERROR,4=CRITICAL)
END
}
