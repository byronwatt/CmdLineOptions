#!/usr/bin/env bats

load "../libs/bats-support/load"
load "../libs/bats-assert/load"


@test "no arguments no output" {
  echo "running build/example_as_string"
  run build/example_as_string
  [ $status -eq 0 ]
  assert_output --stdin <<END
END
}

@test "bad arguments displays help message" {
  run build/example_as_string asdf
  [ $status -eq 255 ]
  assert_output --stdin <<END
no match for 'asdf'

example_as_string
  - this was used for creating a script file.

  some_bool        - testing bool option
  some_alias       - some_bool=%0 some_alias=%0 some_enum=%0 some_int=%0 some_uint=%0 some_int64=%0 some_uint64=%0 some_intrange=%0 some_intList=: %0 %0 some_stringlist: %0 %0 some_double=%0 some_string=%0
  some_enum        - testing some_enum
  some_int         - testing some_int
  some_uint        - testing some_uint
  some_int64       - testing some_int64
  some_uint64      - testing some_uint64
  some_intrange    - testing some_intrange
  some_intList:    - testing some_intList
  some_stringlist: - testing some_stringlist
  some_double      - testing some_double
  some_string      - testing some_string
  log_level        - set SW APPLIB logging level (0=DEBUG,1=INFO,2=WARNING,3=ERROR,4=CRITICAL)
END
}