#!/usr/bin/env bats

load "libs/bats-support/load"
load "libs/bats-assert/load"

@test "intlist - " {
  run build/example some_intList: 1 1 2 3 5 8 13
  [ $status -eq 0 ]
  
  assert_output --stdin <<END
option_some_intList.is_set
option_some_intList: 1 1 2 3 5 8 13
END
}

@test "intlist - with <start>..<stop>" {
  run build/example some_intList: 0..9
  [ $status -eq 0 ]
  assert_output --stdin <<END
option_some_intList.is_set
option_some_intList: 0 1 2 3 4 5 6 7 8 9
END
}

@test "intlist - with <start>..<stop> but bad integer" {
  run build/example some_intList: 0..9f
  [ $status -eq 255 ]
  assert_output --partial "no match for '0..9f'"
}

@test "intlist - skip range <start>+<size>/<skip>" {
  run build/example some_intList: 5+20/5
  [ $status -eq 0 ]
  assert_output --stdin <<END
option_some_intList.is_set
option_some_intList: 5 10 15 20 25 30 35 40 45 50 55 60 65 70 75 80 85 90 95 100
END
}
