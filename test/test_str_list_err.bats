#!/usr/bin/env bats

load "libs/bats-support/load"
load "libs/bats-assert/load"

@test "err string list - " {
  run build/example_with_error_message some_stringlist: one two three
  [ $status -eq 0 ]
  
  assert_output --stdin <<END
option_some_stringlist.is_set
option_some_stringlist: one two three
END
}

@test "err string list - consumes subsequent options" {
  run build/example_with_error_message some_stringlist: one two three some_bool
  [ $status -eq 0 ]
  
  assert_output --stdin <<END
option_some_stringlist.is_set
option_some_stringlist: one two three some_bool
END
}

@test "err string list - stops if it finds another option" {
  run build/example_with_error_message optionfreestringlist: one two three some_bool
  [ $status -eq 0 ]
  
  assert_output --stdin <<END
option_some_bool.is_set
option_some_bool.value = true
option_optionfreestringlist.is_set
option_optionfreestringlist: one two three
END
}
