#!/usr/bin/env bats

load "libs/bats-support/load"
load "libs/bats-assert/load"

@test "alone on the command line is the same as true" {
  run build/example_with_error_message some_bool
  [ $status -eq 0 ]

  assert_output --stdin <<END
option_some_bool.is_set
option_some_bool.value = true
END
}

@test "yes" {
  run build/example_with_error_message some_bool=yEs
  [ $status -eq 0 ]

  assert_output --stdin <<END
option_some_bool.is_set
option_some_bool.value = true
END
}

@test "no" {
  run build/example_with_error_message some_bool=No
  [ $status -eq 0 ]

  assert_output --stdin <<END
option_some_bool.is_set
option_some_bool.value = false
END
}

@test "true" {
  run build/example_with_error_message some_bool=truE
  [ $status -eq 0 ]

  assert_output --stdin <<END
option_some_bool.is_set
option_some_bool.value = true
END
}

@test "false" {
  run build/example_with_error_message some_bool=FalsE
  [ $status -eq 0 ]

  assert_output --stdin <<END
option_some_bool.is_set
option_some_bool.value = false
END
}

@test "on" {
  run build/example_with_error_message some_bool=oN
  [ $status -eq 0 ]

  assert_output --stdin <<END
option_some_bool.is_set
option_some_bool.value = true
END
}

@test "off" {
  run build/example_with_error_message some_bool=oFf
  [ $status -eq 0 ]

  assert_output --stdin <<END
option_some_bool.is_set
option_some_bool.value = false
END
}

@test "0" {
  run build/example_with_error_message some_bool=0
  [ $status -eq 0 ]

  assert_output --stdin <<END
option_some_bool.is_set
option_some_bool.value = false
END
}

@test "1" {
  run build/example_with_error_message some_bool=1
  [ $status -eq 0 ]

  assert_output --stdin <<END
option_some_bool.is_set
option_some_bool.value = true
END
}

@test "2" {
  run build/example_with_error_message some_bool=2
  [ $status -eq 255 ]

  assert_output --partial <<END
error parsing 'some_bool=2'
END
}

# 
@test "invalid string" {
  run build/example_with_error_message some_bool=truthiness
  [ $status -eq 255 ]

  assert_output --stdin <<END
ParseOptionsOrError returned false
error parsing 'truthiness'
 for bool option 'some_bool'
 option description: testing bool option
valid values are 0,1,no,yes,off,on,false,true
error parsing "some_bool=truthiness"
END
}

@test "bool from environment variable" {
  export PROJECT_NAME_some_bool=true
  run build/example_with_error_message
  [ $status -eq 0 ]
  assert_output --stdin <<END
setting some_bool to "true" (from environment variable PROJECT_NAME_some_bool)
option_some_bool.is_set
option_some_bool.value = true
END
}
