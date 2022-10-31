#!/usr/bin/env bats

load "libs/bats-support/load"
load "libs/bats-assert/load"

@test "err enum - integers even out of range" {
  run build/example_with_error_message some_enum=123
  [ $status -eq 0 ]

  assert_output --stdin <<END
option_some_enum.is_set
option_some_enum.value = 123
END
}

# doesn't fail,... just silently returns the bottom 32 bits !!!
@test "err enum - bad enum" {
  run build/example_with_error_message some_enum=bad
  [ $status -eq 255 ]

  assert_output --stdin <<END
ParseOptionsOrError returned false
unknown some_enum "bad"
valid enumerations are: 
  zero  often enums are used to select which test to run
  one   and you have blurbs for what the tests do
  two   you get the idea
  three each enum
  four  has a usage message
error parsing "some_enum=bad"
END
}

@test "err enum - case sensitive enum" {
  run build/example_with_error_message some_enum=two
  [ $status -eq 0 ]
  assert_output --stdin <<END
option_some_enum.is_set
option_some_enum.value = 2
END
}

@test "err enum - case insensitive enum" {
  run build/example_with_error_message some_enum=TwO
  [ $status -eq 0 ]
  assert_output --stdin <<END
option_some_enum.is_set
option_some_enum.value = 2
END
}


@test "err enum - from environment variable" {
  export PROJECT_NAME_some_enum=THREe
  run build/example_with_error_message
  [ $status -eq 0 ]
  assert_output --stdin <<END
setting some_enum to "THREe" (from environment variable PROJECT_NAME_some_enum)
option_some_enum.is_set
option_some_enum.value = 3
END
}
