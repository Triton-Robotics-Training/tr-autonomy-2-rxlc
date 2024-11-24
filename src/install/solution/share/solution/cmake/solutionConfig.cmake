# generated from ament/cmake/core/templates/nameConfig.cmake.in

# prevent multiple inclusion
if(_solution_CONFIG_INCLUDED)
  # ensure to keep the found flag the same
  if(NOT DEFINED solution_FOUND)
    # explicitly set it to FALSE, otherwise CMake will set it to TRUE
    set(solution_FOUND FALSE)
  elseif(NOT solution_FOUND)
    # use separate condition to avoid uninitialized variable warning
    set(solution_FOUND FALSE)
  endif()
  return()
endif()
set(_solution_CONFIG_INCLUDED TRUE)

# output package information
if(NOT solution_FIND_QUIETLY)
  message(STATUS "Found solution: 0.0.0 (${solution_DIR})")
endif()

# warn when using a deprecated package
if(NOT "" STREQUAL "")
  set(_msg "Package 'solution' is deprecated")
  # append custom deprecation text if available
  if(NOT "" STREQUAL "TRUE")
    set(_msg "${_msg} ()")
  endif()
  # optionally quiet the deprecation message
  if(NOT ${solution_DEPRECATED_QUIET})
    message(DEPRECATION "${_msg}")
  endif()
endif()

# flag package as ament-based to distinguish it after being find_package()-ed
set(solution_FOUND_AMENT_PACKAGE TRUE)

# include all config extra files
set(_extras "")
foreach(_extra ${_extras})
  include("${solution_DIR}/${_extra}")
endforeach()
