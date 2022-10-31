#!/usr/bin/env bats

load "libs/bats-support/load"
load "libs/bats-assert/load"

@test "uint with value between max int and max uint" {
  run build/example some_uint64=0x8765432187654321
  [ $status -eq 0 ]
  
  assert_output --stdin <<END
option_some_uint64.is_set
option_some_uint64.value = 9756277979052589857 (0x8765432187654321)
END
}

# doesn't fail,... just silently returns max uint64
@test "uint much bigger than max uint" {
  run build/example some_uint64=0xff1234567812345678
  [ $status -eq 0 ]

  assert_output --stdin <<END
option_some_uint64.is_set
option_some_uint64.value = 18446744073709551615 (0xffffffffffffffff)
END
}

@test "bad uint" {
  run build/example some_uint64=10f
  [ $status -eq 255 ]
  assert_output --partial <<END
error parsing 'some_uint64=10f'
END
}

@test "leading zeros does not convert to octal" {
  run build/example some_uint64=010
  [ $status -eq 0 ]
  assert_output --stdin <<END
option_some_uint64.is_set
option_some_uint64.value = 10 (0xa)
END
}

@test "uint from environment variable" {
  export PROJECT_NAME_some_uint64=123
  run build/example
  [ $status -eq 0 ]
  assert_output --stdin <<END
setting some_uint64 to "123" (from environment variable PROJECT_NAME_some_uint64)
option_some_uint64.is_set
option_some_uint64.value = 123 (0x7b)
END
}
