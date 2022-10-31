#!/usr/bin/env bats

load "libs/bats-support/load"
load "libs/bats-assert/load"


@test "str - string list - " {
  run build/example_as_string some_stringlist: one two three
  [ $status -eq 0 ]
  
  assert_output --stdin <<END
option_some_stringlist.is_set
option_some_stringlist: one two three
END
}

@test "str - string list - consumes subsequent options" {
  run build/example_as_string some_stringlist: one two three some_bool
  [ $status -eq 0 ]
  
  assert_output --stdin <<END
option_some_stringlist.is_set
option_some_stringlist: one two three some_bool
END
}

@test "str - string list - stops if it finds another option" {
  run build/example_as_string optionfreestringlist: one two three some_bool
  [ $status -eq 0 ]
  
  assert_output --stdin <<END
option_some_bool.is_set
option_some_bool.value = true
option_optionfreestringlist.is_set
option_optionfreestringlist: one two three
END
}