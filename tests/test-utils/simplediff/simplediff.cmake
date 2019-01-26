# Copyright (c) 2018 Alain Martin
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

function(diff old_var new_var out_var)

  function(sublist list_var begin end out_var)

    set(in_list ${${list_var}})
    set(out_list)

    set(index ${begin})
    while(index LESS end)
      list(GET in_list ${index} element)

      list(APPEND out_list ${element})

      math(EXPR index "${index} + 1")
    endwhile()

    set(${out_var} ${out_list} PARENT_SCOPE)

  endfunction()

  set(old ${${old_var}})
  list(LENGTH old old_length)

  set(new ${${new_var}})
  list(LENGTH new new_length)

  set(old_values ${old})
  if(old_values)
    list(REMOVE_DUPLICATES old_values)
  endif()

  list(LENGTH old_values old_values_length)
  set(i 0)
  while(i LESS old_values_length)
    list(GET old_values ${i} val)

    set(indices)
    set(sub ${old})
    set(sub_begin 0)
    set(sub_found 0)

    while(NOT sub_found EQUAL -1)
      list(FIND sub ${val} sub_found)

      if(NOT sub_found EQUAL -1)
        math(EXPR real_index "${sub_begin} + ${sub_found}")
        list(APPEND indices ${real_index})

        math(EXPR sub_begin "${real_index} + 1")
        sublist(old ${sub_begin} ${old_length} sub)
      endif()
    endwhile()

    set(old_index_map_${i} ${indices})

    math(EXPR i "${i} + 1")
  endwhile()

  set(overlap_keys)
  set(overlap_values)

  set(sub_start_old 0)
  set(sub_start_new 0)
  set(sub_length 0)

  set(i_new 0)
  while(i_new LESS new_length)
    list(GET new ${i_new} val)

    set(_overlap_keys)
    set(_overlap_values)

    list(FIND old_values "${val}" old_values_index)
    foreach(i_old IN LISTS old_index_map_${old_values_index})
      if(i_old EQUAL 0)
        set(_overlap_val 1)
      else()
        math(EXPR i_old_minus_one "${i_old} - 1")
        list(FIND overlap_keys ${i_old_minus_one} found_index)
        if(found_index EQUAL -1)
          set(_overlap_val 1)
        else()
          list(GET overlap_values ${found_index} overlap_val)
          math(EXPR overlap_val_plus_one "${overlap_val} + 1")
          set(_overlap_val ${overlap_val_plus_one})
        endif()
      endif()
      list(APPEND _overlap_keys ${i_old})
      list(APPEND _overlap_values ${_overlap_val})

      if(_overlap_val GREATER sub_length)
        set(sub_length ${_overlap_val})
        math(EXPR sub_start_old "${i_old} - ${sub_length} + 1")
        math(EXPR sub_start_new "${i_new} - ${sub_length} + 1")
      endif()
    endforeach()

    set(overlap_keys ${_overlap_keys})
    set(overlap_values ${_overlap_values})

    math(EXPR i_new "${i_new} + 1")
  endwhile()

  set(out)

  if(sub_length EQUAL 0)
    if(NOT old_length EQUAL 0)
      list(APPEND out "-${old_length}" ${old})
    endif()
    if(NOT new_length EQUAL 0)
      list(APPEND out "+${new_length}" ${new})
    endif()
  else()
    sublist(old 0 ${sub_start_old} left_sublist_old)
    sublist(new 0 ${sub_start_new} left_sublist_new)
    diff(left_sublist_old left_sublist_new left_diff)
    list(APPEND out ${left_diff})

    math(EXPR right_sub_start_new "0${sub_start_new} + ${sub_length}")
    sublist(new ${sub_start_new} ${right_sub_start_new} common_sublist)
    list(APPEND out "=${sub_length}" ${common_sublist})

    math(EXPR right_sub_start_old "0${sub_start_old} + ${sub_length}")
    sublist(old ${right_sub_start_old} ${old_length} right_sublist_old)
    sublist(new ${right_sub_start_new} ${new_length} right_sublist_new)
    diff(right_sublist_old right_sublist_new right_diff)
    list(APPEND out ${right_diff})
  endif()

  set(${out_var} ${out} PARENT_SCOPE)

endfunction()


function(print_diff diff_var)

  set(sub_diff_index 0)
  set(sub_diff_length 0)

  string(ASCII 27 escape)
  if(NOT WIN32)
    set(color_reset "${escape}[m")
    set(red "${escape}[31m")
    set(green "${escape}[32m")
  endif()

  foreach(item IN LISTS ${diff_var})
    if(sub_diff_index LESS sub_diff_length)
      if(sub_diff_type STREQUAL "=")
        message("  ${item}")
      elseif(sub_diff_type STREQUAL "-")
        message("${red}- ${item}${color_reset}")
      elseif(sub_diff_type STREQUAL "+")
        message("${green}+ ${item}${color_reset}")
      else()
        message(FATAL_ERROR "Unknown diff type \"${sub_diff_type}\"")
      endif()

      math(EXPR sub_diff_index "${sub_diff_index} + 1")
    else()
      string(REGEX MATCH "([+=-])([0-9]+)" m "${item}")
      if(NOT m)
        message(FATAL_ERROR "Invalid diff result")
      endif()
      set(sub_diff_type "${CMAKE_MATCH_1}")
      set(sub_diff_length "${CMAKE_MATCH_2}")
      set(sub_diff_index 0)
    endif()
  endforeach()

endfunction()
