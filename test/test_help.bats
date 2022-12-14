#!/usr/bin/env bats

load "libs/bats-support/load"
load "libs/bats-assert/load"


@test "help - no arguments no output" {
  run build/example
  [ $status -eq 0 ]
  assert_output --stdin <<END
END
}

@test "help - corner case - null string argument fails" {
  run build/example ''
  [ $status -eq 255 ]
  assert_output --partial <<END
  no match for ''
END
}

@test "help - bad arguments displays help message" {
  run build/example asdf
  [ $status -eq 255 ]
  assert_output --stdin <<END
no match for 'asdf'

example
  - normal parsing, asserts if a bad option is found

  some_bool             - testing bool option
  some_enum             - testing some_enum
  some_int              - testing some_int
  some_uint             - testing some_uint
  some_int64            - testing some_int64
  some_uint64           - testing some_uint64
  some_intrange         - testing some_intrange
  some_intList:         - testing some_intList
  some_stringlist:      - testing some_stringlist
  optionfreestringlist: - testing optionfreestringlist (valid option terminates list)
  some_double           - testing some_double
  some_string           - testing some_string
END
}
