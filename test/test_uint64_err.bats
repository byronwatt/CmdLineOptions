#!/usr/bin/env bats

load "libs/bats-support/load"
load "libs/bats-assert/load"

@test "err uint64 - with value between max int and max uint" {
  run build/example_with_error_message some_uint64=0x8765432187654321
  [ $status -eq 0 ]
  
  assert_output --stdin <<END
option_some_uint64.is_set
option_some_uint64.value = 9756277979052589857 (0x8765432187654321)
END
}

# doesn't fail,... just silently returns max uint64
@test "err uint64 - much bigger than max uint" {
  run build/example_with_error_message some_uint64=0xff1234567812345678
  [ $status -eq 0 ]

  assert_output --stdin <<END
option_some_uint64.is_set
option_some_uint64.value = 18446744073709551615 (0xffffffffffffffff)
END
}

@test "err uint64 - bad uint" {
  run build/example_with_error_message some_uint64=10f
  [ $status -eq 255 ]
  assert_output --stdin <<END
ParseOptionsOrError returned false
error parsing '10f'
 for uint64 option 'some_uint64'
 option description: testing some_uint64
error parsing "some_uint64=10f"
END
}

@test "err uint64 - leading zeros does not convert to octal" {
  run build/example_with_error_message some_uint64=010
  [ $status -eq 0 ]
  assert_output --stdin <<END
option_some_uint64.is_set
option_some_uint64.value = 10 (0xa)
END
}

@test "err uint64 - from environment variable" {
  export PROJECT_NAME_some_uint64=123
  run build/example_with_error_message
  [ $status -eq 0 ]
  assert_output --stdin <<END
setting some_uint64 to "123" (from environment variable PROJECT_NAME_some_uint64)
option_some_uint64.is_set
option_some_uint64.value = 123 (0x7b)
END
}
