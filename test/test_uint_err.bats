#!/usr/bin/env bats

load "libs/bats-support/load"
load "libs/bats-assert/load"

@test "err uint - with value between max int and max uint" {
  run build/example_with_error_message some_uint=0x87654321
  [ $status -eq 0 ]
  
  assert_output --stdin <<END
option_some_uint.is_set
option_some_uint.value = 2271560481 (0x87654321)
END
}

# doesn't fail,... just silently returns the bottom 32 bits
@test "err uint - much bigger than max uint" {
  run build/example_with_error_message some_uint=0xff12345678
  [ $status -eq 0 ]

  assert_output --stdin <<END
option_some_uint.is_set
option_some_uint.value = 305419896 (0x12345678)
END
}

@test "err uint - bad uint" {
  run build/example_with_error_message some_uint=10f
  [ $status -eq 255 ]
  assert_output --stdin <<END
ParseOptionsOrError returned false
error parsing '10f'
 for uint option 'some_uint'
 option description: testing some_uint
error parsing "some_uint=10f"
END
}

@test "err uint - leading zeros does not convert to octal" {
  run build/example_with_error_message some_uint=010
  [ $status -eq 0 ]
  assert_output --stdin <<END
option_some_uint.is_set
option_some_uint.value = 10 (0xa)
END
}

@test "err uint - from environment variable" {
  export PROJECT_NAME_some_uint=123
  run build/example_with_error_message
  [ $status -eq 0 ]
  assert_output --stdin <<END
setting some_uint to "123" (from environment variable PROJECT_NAME_some_uint)
option_some_uint.is_set
option_some_uint.value = 123 (0x7b)
END
}
