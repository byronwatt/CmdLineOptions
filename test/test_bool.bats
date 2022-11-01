#!/usr/bin/env bats

load "libs/bats-support/load"
load "libs/bats-assert/load"

@test "bool - alone on the command line is the same as true" {
  run build/example some_bool
  [ $status -eq 0 ]

  assert_output --stdin <<END
option_some_bool.is_set
option_some_bool.value = true
END
}

@test "bool - yes" {
  run build/example -some_bool=yEs
  [ $status -eq 0 ]

  assert_output --stdin <<END
option_some_bool.is_set
option_some_bool.value = true
END
}

@test "bool - no" {
  run build/example --some_bool=No
  [ $status -eq 0 ]

  assert_output --stdin <<END
option_some_bool.is_set
option_some_bool.value = false
END
}

@test "bool - true" {
  run build/example some_bool=truE
  [ $status -eq 0 ]

  assert_output --stdin <<END
option_some_bool.is_set
option_some_bool.value = true
END
}

@test "bool - false" {
  run build/example some_bool=FalsE
  [ $status -eq 0 ]

  assert_output --stdin <<END
option_some_bool.is_set
option_some_bool.value = false
END
}

@test "bool - on" {
  run build/example some_bool=oN
  [ $status -eq 0 ]

  assert_output --stdin <<END
option_some_bool.is_set
option_some_bool.value = true
END
}

@test "bool - off" {
  run build/example some_bool=oFf
  [ $status -eq 0 ]

  assert_output --stdin <<END
option_some_bool.is_set
option_some_bool.value = false
END
}

@test "bool - 0" {
  run build/example some_bool=0
  [ $status -eq 0 ]

  assert_output --stdin <<END
option_some_bool.is_set
option_some_bool.value = false
END
}

@test "bool - 1" {
  run build/example some_bool=1
  [ $status -eq 0 ]

  assert_output --stdin <<END
option_some_bool.is_set
option_some_bool.value = true
END
}

@test "bool - 2 (fails)" {
  run build/example some_bool=2
  [ $status -eq 255 ]

  assert_output --partial <<END
error parsing 'some_bool=2'
END
}

# 
@test "bool - invalid string" {
  run build/example some_bool=truthiness
  [ $status -eq 255 ]

  assert_output --stdin <<END
error parsing 'some_bool=truthiness'

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

@test "bool from environment variable" {
  export PROJECT_NAME_some_bool=true
  run build/example
  [ $status -eq 0 ]
  assert_output --stdin <<END
setting some_bool to "true" (from environment variable PROJECT_NAME_some_bool)
option_some_bool.is_set
option_some_bool.value = true
END
}
