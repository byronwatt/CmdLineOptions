#!/usr/bin/env bats

load "../libs/bats-support/load"
load "../libs/bats-assert/load"

# doesn't fail,... just silently returns max int64
@test "int bigger than max int" {
  run build/example_as_string some_int64=9223372036854775809
  [ $status -eq 0 ]

  assert_output --stdin <<END
option_some_int64.is_set
option_some_int64.value = 9223372036854775807 (0x7fffffffffffffff)
END
}

# doesn't fail,... just silently returns max int64
@test "int much bigger than max int" {
  run build/example_as_string some_int64=0xff1234567812345678
  [ $status -eq 0 ]

  assert_output --stdin <<END
option_some_int64.is_set
option_some_int64.value = 9223372036854775807 (0x7fffffffffffffff)
END
}

@test "bad int" {
  run build/example_as_string some_int64=10f
  [ $status -eq 255 ]
  assert_output --partial <<END
error parsing 'some_int64=10f'
END
}

@test "leading zeros does not convert to octal" {
  run build/example_as_string some_int64=010
  [ $status -eq 0 ]
  assert_output --stdin <<END
option_some_int64.is_set
option_some_int64.value = 10 (0xa)
END
}

@test "int from environment variable" {
  export PROJECT_NAME_some_int64=123
  run build/example_as_string
  [ $status -eq 0 ]
  assert_output --stdin <<END
setting some_int64 to "123" (from environment variable PROJECT_NAME_some_int64)
option_some_int64.is_set
option_some_int64.value = 123 (0x7b)
END
}
