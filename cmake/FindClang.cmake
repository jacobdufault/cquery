#.rst
# FindClang
# ---------
#
# Find Clang and LLVM libraries required by cquery
#
# Results are reported in the following variables::
#
#   Clang_FOUND         - True if headers and requested libraries were found
#   Clang_EXECUTABLE    - Clang executable
#   Clang_FORMAT        - Clang-format executable
#   Clang_RESOURCE_DIR  - Clang resource directory
#   Clang_VERSION       - Clang version as reported by Clang executable
#
# The following :prop_tgt:`IMPORTED` targets are also defined::
#
#   Clang::Clang        - Target for all required Clang libraries and headers
#
# This module reads hints about which libraries to look for and where to find
# them from the following variables::
#
#   CLANG_ROOT          - If set, only look for Clang components in CLANG_ROOT
#
# Example to link against Clang target::
#
#   target_link_libraries(<target> PRIVATE Clang::Clang)

### Definitions

# Wrapper macro's around the find_* macro's from CMake that only search in
# CLANG_ROOT if it is defined

macro(_Clang_find_library VAR NAME)
  # Windows needs lib prefix
  if (CLANG_ROOT)    
    find_library(${VAR} NAMES ${NAME} lib${NAME} 
                 NO_DEFAULT_PATH PATHS ${CLANG_ROOT} PATH_SUFFIXES lib)
  else()
    find_library(${VAR} NAMES ${NAME} lib${NAME})
  endif()
endmacro()

macro(_Clang_find_path VAR INCLUDE_FILE)
  if (CLANG_ROOT)
    find_path(${VAR} ${INCLUDE_FILE} 
              NO_DEFAULT_PATH PATHS ${CLANG_ROOT} PATH_SUFFIXES include)
  else()
    find_path(${VAR} ${INCLUDE_FILE})
  endif()
endmacro()

macro(_Clang_find_program VAR NAME)
  if (CLANG_ROOT)
    find_program(${VAR} ${NAME} 
                 NO_DEFAULT_PATH PATHS ${CLANG_ROOT} PATH_SUFFIXES bin)
  else()
    find_program(${VAR} ${NAME})
  endif()
endmacro()

### Start

set(_Clang_REQUIRED_VARS _libclang_LIBRARY _libclang_INCLUDE_DIR 
                         Clang_EXECUTABLE Clang_RESOURCE_DIR Clang_VERSION)

_Clang_find_library(_libclang_LIBRARY clang)
_Clang_find_path(_libclang_INCLUDE_DIR clang-c/Index.h)

_Clang_find_program(Clang_FORMAT clang-format)
_Clang_find_program(Clang_EXECUTABLE clang)

if(Clang_EXECUTABLE)
  # Find Clang resource directory with Clang executable
  execute_process(COMMAND ${Clang_EXECUTABLE} -print-resource-dir
                  RESULT_VARIABLE _Clang_FIND_RESOURCE_DIR_RESULT
                  OUTPUT_VARIABLE Clang_RESOURCE_DIR
                  ERROR_VARIABLE _Clang_FIND_RESOURCE_DIR_ERROR
                  OUTPUT_STRIP_TRAILING_WHITESPACE)

  if(_Clang_FIND_RESOURCE_DIR_RESULT)
    message(FATAL_ERROR "Error retrieving Clang resource directory with Clang \
executable. Output:\n ${_Clang_FIND_RESOURCE_DIR_ERROR}")
  endif()

  # Find Clang version
  set(_Clang_VERSION_REGEX "([0-9]+)\\.([0-9]+)\\.([0-9]+)")
  execute_process(COMMAND ${Clang_EXECUTABLE} --version 
                  OUTPUT_VARIABLE Clang_VERSION)
  string(REGEX MATCH ${_Clang_VERSION_REGEX} Clang_VERSION ${Clang_VERSION})
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Clang
                                  FOUND_VAR Clang_FOUND
                                  REQUIRED_VARS ${_Clang_REQUIRED_VARS}
                                  VERSION_VAR Clang_VERSION)

if(Clang_FOUND AND NOT TARGET Clang::Clang)
  add_library(Clang::Clang UNKNOWN IMPORTED)
  set_target_properties(Clang::Clang PROPERTIES
                        IMPORTED_LOCATION ${_libclang_LIBRARY}
                        INTERFACE_INCLUDE_DIRECTORIES ${_libclang_INCLUDE_DIR})
endif()
