#!/usr/bin/env bats

# fixtures bats


@test "no arguments no output" {
  echo "running build/example"
  run build/example
  [ $status -eq 0 ]
  # [ $result -eq "" ]
}



local expected_result
expected_result=$(cat <<END
no match for 'asdf'
  log_level        - set SW APPLIB logging level (0=DEBUG,1=INFO,2=WARNING,3=ERROR,4=CRITICAL)
  some_bool        - testing bool option
  some_alias       - some_bool=true some_enum=two some_int=3 some_double=4
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
END
)

@test "bad arguments displays help message" {
  echo "running build/example asdf"
  echo "and hoping output is:"
  echo $expected_result
  run build/example asdf
  [ $status -eq 255 ]
  [ $result -eq $expected_result ]
}

expected_result=$(cat <<END
option_some_uint.is_set
option_some_uint.value = 2271560481 (0x87654321)
END
)

@test "uint" {
  echo "running build/example asdf"
  echo "and hoping output is:"
  echo $expected_result
  run build/example some_uint=0x87654321
  [ $status -eq 0 ]
  [ $result -eq $expected_result ]
}
