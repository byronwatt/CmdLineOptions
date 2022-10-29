#!/usr/bin/env bats

load "libs/bats-support/load"
load "libs/bats-assert/load"

# note: the %0 .. %x replacements are not working for aliases !
# (something got lost in the translation !!!!)

# @test "alias 0" {
#   echo "running build/example"
#   run build/example some_alias 0
#   [ $status -eq 0 ]
#   assert_output --stdin <<END
# END
# }
# 
# @test "alias 1" {
#   run build/example some_alias 1
#   [ $status -eq 0 ]
#   assert_output --stdin <<END
# END
# }
# 
# @test "alias invalid" {
#   run build/example some_alias bad
#   [ $status -eq 255 ]
#   assert_output --stdin <<END
# END
# }
