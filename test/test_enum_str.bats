#!/usr/bin/env bats

load "libs/bats-support/load"
load "libs/bats-assert/load"

@test "str enum - integers even out of range" {
  run build/example_as_string some_enum=123
  [ $status -eq 0 ]

  assert_output --stdin <<END
option_some_enum.is_set
option_some_enum.value = 123 ("undefined")
END
}

# doesn't fail,... just silently returns the bottom 32 bits !!!
@test "str enum - bad enum" {
  run build/example_as_string some_enum=bad
  [ $status -eq 255 ]

  assert_output --stdin <<END
unknown some_enum "bad"
valid enumerations are: 
  zero  0x00 often enums are used to select which test to run
  one   0x01 and you have blurbs for what the tests do
  two   0x02 you get the idea
  three 0x03 each enum
  four  0x04 has a usage message
END
}

@test "str enum - case sensitive enum" {
  run build/example_as_string some_enum=two
  [ $status -eq 0 ]
  assert_output --stdin <<END
option_some_enum.is_set
option_some_enum.value = 2 ("two")
END
}

@test "str enum - case insensitive enum" {
  run build/example_as_string some_enum=TwO
  [ $status -eq 0 ]
  assert_output --stdin <<END
option_some_enum.is_set
option_some_enum.value = 2 ("two")
END
}


@test "str enum - from environment variable" {
  export PROJECT_NAME_some_enum=THREe
  run build/example_as_string
  [ $status -eq 0 ]
  assert_output --stdin <<END
setting some_enum to "THREe" (from environment variable PROJECT_NAME_some_enum)
option_some_enum.is_set
option_some_enum.value = 3 ("three")
END
}
