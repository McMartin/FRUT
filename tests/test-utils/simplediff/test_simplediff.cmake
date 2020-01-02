# Copyright (C) 2018  Alain Martin
#
# This file is part of FRUT.
#
# FRUT is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# FRUT is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with FRUT.  If not, see <http://www.gnu.org/licenses/>.

# CMake implementation of SimpleDiff 1.0 (https://github.com/paulgb/simplediff)
# Copyright (c) 2008 - 2013 Paul Butler and contributors

include("${CMAKE_CURRENT_LIST_DIR}/simplediff.cmake")


function(assert_equal a b)

  if(NOT a STREQUAL b)
    message(SEND_ERROR "${a} != ${b}")
  endif()

endfunction()


function(test_delete_diff)
  message(STATUS "test_delete_diff")

  set(old 1 2 3 4 5)
  set(new 1 2 5)
  set(expected_diff "=2" 1 2 "-2" 3 4 "=1" 5)
  diff(old new actual_diff)
  assert_equal("${actual_diff}" "${expected_diff}")

  set(old 1 2 3 4 5 6 7 8)
  set(new 3 6 7)
  set(expected_diff "-2" 1 2 "=1" 3 "-2" 4 5 "=2" 6 7 "-1" 8)
  diff(old new actual_diff)
  assert_equal("${actual_diff}" "${expected_diff}")

  set(old 1 2 3 4 5 1 2 3 4 5)
  set(new 1 2 3 4 5)
  set(expected_diff "=5" 1 2 3 4 5 "-5" 1 2 3 4 5)
  diff(old new actual_diff)
  assert_equal("${actual_diff}" "${expected_diff}")

endfunction()


function(test_insert_diff)
  message(STATUS "test_insert_diff")

  set(old 1 3 4)
  set(new 1 2 3 4)
  set(expected_diff "=1" 1 "+1" 2 "=2" 3 4)
  diff(old new actual_diff)
  assert_equal("${actual_diff}" "${expected_diff}")

  set(old 1 2 3 8 9 12 13)
  set(new 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15)
  set(expected_diff "=3" 1 2 3 "+4" 4 5 6 7 "=2" 8 9 "+2" 10 11 "=2" 12 13 "+2" 14 15)
  diff(old new actual_diff)
  assert_equal("${actual_diff}" "${expected_diff}")

  set(old 1 2 3 4 5)
  set(new 1 2 2 3 4 5)
  set(expected_diff "=1" 1 "+1" 2 "=4" 2 3 4 5)
  diff(old new actual_diff)
  assert_equal("${actual_diff}" "${expected_diff}")

  set(old 1 2 3 4 5)
  set(new 1 2 2 3 4 4 5)
  set(expected_diff "=1" 1 "+1" 2 "=3" 2 3 4 "+1" 4 "=1" 5)
  diff(old new actual_diff)
  assert_equal("${actual_diff}" "${expected_diff}")

  set(old 1 2 3 4 5)
  set(new 1 2 1 2 3 3 2 1 4 5)
  set(expected_diff "+2" 1 2 "=3" 1 2 3 "+3" 3 2 1 "=2" 4 5)
  diff(old new actual_diff)
  assert_equal("${actual_diff}" "${expected_diff}")

endfunction()


function(test_words_diff)
  message(STATUS "test_words_diff")

  set(old "The" "quick" "brown" "fox")
  set(new "The" "slow" "green" "turtle")
  set(expected_diff "=1" "The" "-3" "quick" "brown" "fox" "+3" "slow" "green" "turtle")
  diff(old new actual_diff)
  assert_equal("${actual_diff}" "${expected_diff}")

  set(old "jumps" "over" "the" "lazy" "dog")
  set(new "walks" "around" "the" "orange" "cat")
  set(expected_diff "-2" "jumps" "over" "+2" "walks" "around" "=1" "the" "-2" "lazy" "dog" "+2" "orange" "cat")
  diff(old new actual_diff)
  assert_equal("${actual_diff}" "${expected_diff}")

endfunction()


function(test_character_diff)
  message(STATUS "test_character_diff")

  set(old "T;h;e; ;q;u;i;c;k; ;b;r;o;w;n; ;f;o;x;.")
  set(new "T;h;e; ;k;u;i;c;k; ;b;r;o;w;n; ;f;i;x;.")
  set(expected_diff "=4" "T;h;e; " "-1" "q" "+1" "k" "=12" "u;i;c;k; ;b;r;o;w;n; ;f" "-1" "o" "+1" "i" "=2" "x;.")
  diff(old new actual_diff)
  assert_equal("${actual_diff}" "${expected_diff}")

endfunction()


if(CMAKE_SCRIPT_MODE_FILE STREQUAL CMAKE_CURRENT_LIST_FILE)
  test_delete_diff()
  test_insert_diff()
  test_words_diff()
  test_character_diff()
endif()
