cmake_minimum_required(VERSION 3.1)
project(cquery LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/)
include(DefaultCMakeBuildType)

# Required Clang version
set(CLANG_VERSION 6.0.0 CACHE STRING "Clang version")
set(CLANG_DOWNLOAD_LOCATION ${CMAKE_BINARY_DIR}
    CACHE STRING "Downloaded Clang location")
option(SYSTEM_CLANG "Use system installation of Clang instead of \
       downloading Clang" OFF)
option(ASAN "Compile with address sanitizers" OFF)

# Sources for the executable are specified at end of CMakeLists.txt
add_executable(cquery "")

### Compile options

# CMake default compile flags:
# MSVC + Clang(Windows):
#   debug: /MDd /Zi /Ob0 /Od /RTC1
#   release: /MD /O2 /Ob2 /DNDEBUG
# GCC + Clang(Linux):
#   debug: -g
#   release: -O3 -DNDEBUG

# Enable C++14 (Required)
set_property(TARGET cquery PROPERTY CXX_STANDARD 14)
set_property(TARGET cquery PROPERTY CXX_STANDARD_REQUIRED ON)
# Disable gnu extensions except for Cygwin which needs them to build properly
if(NOT CYGWIN)
  set_property(TARGET cquery PROPERTY CXX_EXTENSIONS OFF)
endif()

# CMake sets MSVC for both MSVC and Clang(Windows)
if(MSVC)
  # Common MSVC/Clang(Windows) options
  target_compile_options(cquery PRIVATE
                         /nologo
                         /EHsc
                         /D_CRT_SECURE_NO_WARNINGS # don't try to use MSVC std replacements
                         /W3 # roughly -Wall
                         /wd4996 # disable loguru unsafe warnings
                         /wd4722 # ignores warning C4722
                                 # (destructor never returns) in loguru
                         /wd4267 # ignores warning C4267
                                 # (conversion from 'size_t' to 'type'),
                                 # roughly -Wno-sign-compare
                         /wd4800
                         $<$<CONFIG:Debug>:/FS>
                         )
else()
  # Common GCC/Clang(Linux) options
  target_compile_options(cquery PRIVATE
                         -Wall
                         -Wno-sign-compare
                         )

  if(${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
    target_compile_options(cquery PRIVATE -Wno-return-type -Wno-unused-result)
  endif()

  if(${CMAKE_CXX_COMPILER_ID} STREQUAL Clang)
    target_compile_options(cquery PRIVATE
                           $<$<CONFIG:Debug>:-fno-limit-debug-info>)
  endif()

  if(ASAN)
    target_compile_options(cquery PRIVATE -fsanitize=address,undefined)
    # target_link_libraries also takes linker flags
    target_link_libraries(cquery PRIVATE -fsanitize=address,undefined)
  endif()
endif()

### Download Clang if required

if(NOT SYSTEM_CLANG)
  message(STATUS "Using downloaded Clang")

  include(DownloadAndExtractClang)
  download_and_extract_clang(${CLANG_VERSION} ${CLANG_DOWNLOAD_LOCATION})
  # Used by FindClang
  set(CLANG_ROOT ${DOWNLOADED_CLANG_DIR})
else()
  message(STATUS "Using system Clang")
endif()

### Libraries

# See cmake/FindClang.cmake
find_package(Clang ${CLANG_VERSION} REQUIRED)
target_link_libraries(cquery PRIVATE Clang::Clang)

# Enable threading support
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(cquery PRIVATE Threads::Threads)

if(${CMAKE_SYSTEM_NAME} STREQUAL Linux)
  # loguru calls dladdr
  target_link_libraries(cquery PRIVATE ${CMAKE_DL_LIBS})

elseif(${CMAKE_SYSTEM_NAME} STREQUAL FreeBSD)
  # loguru::stacktrace_as_stdstring calls backtrace_symbols
  # sparsepp/spp_memory.h uses libkvm
  # src/platform_posix.cc uses libthr
  find_package(Backtrace REQUIRED)
  target_link_libraries(cquery PRIVATE ${Backtrace_LIBRARIES} kvm thr)

elseif(${CMAKE_SYSTEM_NAME} STREQUAL Windows)
  # sparsepp/spp_memory.h uses LibPsapi
  target_link_libraries(cquery PRIVATE Psapi)
endif()

### Definitions

target_compile_definitions(cquery PRIVATE
                           LOGURU_WITH_STREAMS=1
                           LOGURU_FILENAME_WIDTH=18
                           LOGURU_THREADNAME_WIDTH=13
                           DEFAULT_RESOURCE_DIRECTORY="${Clang_RESOURCE_DIR}")

### Includes

target_include_directories(cquery PRIVATE
                           src
                           third_party
                           third_party/doctest
                           third_party/loguru
                           third_party/msgpack-c/include
                           third_party/pugixml/src
                           third_party/rapidjson/include
                           third_party/sparsepp)

### Install

install(TARGETS cquery RUNTIME DESTINATION bin)

# If downloaded Clang is used we have to bundle the required files from 
# the downloaded Clang along with the cquery executable
if(NOT SYSTEM_CLANG)

  # On Linux/FreeBSD/Darwin we set the rpath so cquery can find 
  # libclang.[so,dylib]. On Windows we install libclang.dll to the bin directory
  # to do the same.

  if(${CMAKE_SYSTEM_NAME} MATCHES Linux|FreeBSD)
    set_property(TARGET cquery APPEND PROPERTY
                 INSTALL_RPATH $ORIGIN/../lib)
  elseif(${CMAKE_SYSTEM_NAME} STREQUAL Darwin)
    set_property(TARGET cquery APPEND PROPERTY
                 INSTALL_RPATH @loader_path/../lib)
  elseif(${CMAKE_SYSTEM_NAME} STREQUAL Windows)
    install(FILES ${DOWNLOADED_CLANG_DIR}/bin/libclang.dll DESTINATION bin) 
  endif()

  # Install libclang.[so,lib,dylib] to lib directory

  file(GLOB LIBCLANG_PLUS_SYMLINKS
       ${DOWNLOADED_CLANG_DIR}/lib/libclang.[so,lib,dylib]*)
  install(FILES ${LIBCLANG_PLUS_SYMLINKS} DESTINATION lib)

  # Install cquery-clang, cquery-clang-format to bin directory

  # When installing files the .exe suffix needs to be added on Windows.
  if(${CMAKE_SYSTEM_NAME} STREQUAL Windows)
    set(EXE ".exe")
  else()
    set(EXE "")
  endif()

  # ${Clang_EXECUTABLE} may be a symlink, resolve it before passing to install.
  get_filename_component(Clang_EXECUTABLE_REALPATH ${Clang_EXECUTABLE}
                         REALPATH)

  # Install cquery-clang, cquery-clang-format to install directory.
  install(FILES ${Clang_EXECUTABLE_REALPATH}
          PERMISSIONS OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE
          DESTINATION bin
          RENAME cquery-clang${EXE})
  install(FILES ${Clang_FORMAT}
          PERMISSIONS OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE
          DESTINATION bin
          RENAME cquery-clang-format${EXE})

  # cquery-clang will search for these directories to resolve system includes.
  # We need to install them.
  #
  #  - (which cquery-clang)/../lib/clang/6.0.0/include (or current clang version)
  #  - (which cquery-clang)/../include/c++/v1

  # lib/clang/*/include changes with the version, ie, lib/clang/6.0.0/include.
  # Find the current value for * and then remove the ${DOWNLOADED_CLANG_DIR}
  # prefix.

  file(GLOB CLANG_INCLUDE_DIR ${DOWNLOADED_CLANG_DIR}/lib/clang/*/include)
  string(REPLACE ${DOWNLOADED_CLANG_DIR}/ "" 
         CLANG_INCLUDE_DIR ${CLANG_INCLUDE_DIR})

  # Add trailing slash to overwrite destination directory instead of putting the
  # directory inside the destination directory
  install(DIRECTORY ${DOWNLOADED_CLANG_DIR}/${CLANG_INCLUDE_DIR}/
          DESTINATION ${CLANG_INCLUDE_DIR})
  
  # include/c++/v1 is not included in every Clang download (Windows) so we check 
  # if it exists first
  if(IS_DIRECTORY ${DOWNLOADED_CLANG_DIR}/include/c++/v1)
    install(DIRECTORY ${DOWNLOADED_CLANG_DIR}/include/c++/v1/
            DESTINATION include/c++/v1)
  endif()
endif()

### Tools

# We use glob here since source files are already manually added with
# target_sources further down
file(GLOB SOURCES src/*.cc src/*.h src/serializers/*.cc src/serializers/*.h
                  src/messages/*.h src/messages/*.cc)

if(Clang_FORMAT AND ${Clang_VERSION} STREQUAL 6.0.0)
  add_custom_target(format
                    COMMAND ${Clang_FORMAT} -i ${SOURCES}
                    # .clang-format is located in the cquery root project dir
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                    COMMENT "Running clang-format ...")
else()
  # Set error message depending on which condition was false
  if (NOT Clang_FORMAT)
    set(Clang_FORMAT_ERROR "Error: clang-format executable not found")
  elseif(NOT ${Clang_VERSION} STREQUAL 6.0.0)
    set(Clang_FORMAT_ERROR "Error: clang-format version does not match \
6.0.0. Due to differences in clang-format output between versions we only \
support clang-format 6.0.0")
  endif()

  add_custom_target(format
                    COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --red --bold
                    ${Clang_FORMAT_ERROR})
endif()

### Sources

target_sources(cquery PRIVATE
               third_party/siphash.cc
               third_party/pugixml/src/pugixml.cpp)

target_sources(cquery PRIVATE
               src/cache_manager.cc
               src/clang_complete.cc
               src/clang_cursor.cc
               src/clang_format.cc
               src/clang_index.cc
               src/clang_indexer.cc
               src/clang_system_include_extractor.cc
               src/clang_translation_unit.cc
               src/clang_utils.cc
               src/code_complete_cache.cc
               src/command_line.cc
               src/compiler.cc
               src/diagnostics_engine.cc
               src/file_consumer.cc
               src/file_contents.cc
               src/file_types.cc
               src/fuzzy_match.cc
               src/iindexer.cc
               src/import_manager.cc
               src/import_pipeline.cc
               src/include_complete.cc
               src/method.cc
               src/lex_utils.cc
               src/lsp.cc
               src/lsp_diagnostic.cc
               src/match.cc
               src/message_handler.cc
               src/options.cc
               src/platform_posix.cc
               src/platform_win.cc
               src/platform.cc
               src/position.cc
               src/project.cc
               src/query_utils.cc
               src/query.cc
               src/queue_manager.cc
               src/recorder.cc
               src/semantic_highlight_symbol_cache.cc
               src/serializer.cc
               src/standard_includes.cc
               src/task.cc
               src/test.cc
               src/third_party_impl.cc
               src/threaded_queue.cc
               src/timer.cc
               src/timestamp_manager.cc
               src/type_printer.cc
               src/utils.cc
               src/work_thread.cc
               src/working_files.cc)

target_sources(cquery PRIVATE
               src/messages/cquery_base.cc
               src/messages/cquery_call_hierarchy.cc
               src/messages/cquery_callers.cc
               src/messages/cquery_did_view.cc
               src/messages/cquery_file_info.cc
               src/messages/cquery_freshen_index.cc
               src/messages/cquery_index_file.cc
               src/messages/cquery_inheritance_hierarchy.cc
               src/messages/cquery_member_hierarchy.cc
               src/messages/cquery_vars.cc
               src/messages/cquery_wait.cc
               src/messages/exit.cc
               src/messages/initialize.cc
               src/messages/shutdown.cc
               src/messages/text_document_code_action.cc
               src/messages/text_document_code_lens.cc
               src/messages/text_document_completion.cc
               src/messages/text_document_definition.cc
               src/messages/text_document_did_change.cc
               src/messages/text_document_did_close.cc
               src/messages/text_document_did_open.cc
               src/messages/text_document_did_save.cc
               src/messages/text_document_document_highlight.cc
               src/messages/text_document_document_link.cc
               src/messages/text_document_document_symbol.cc
               src/messages/text_document_formatting.cc
               src/messages/text_document_hover.cc
               src/messages/text_document_implementation.cc
               src/messages/text_document_range_formatting.cc
               src/messages/text_document_references.cc
               src/messages/text_document_rename.cc
               src/messages/text_document_signature_help.cc
               src/messages/text_document_type_definition.cc
               src/messages/workspace_did_change_configuration.cc
               src/messages/workspace_did_change_watched_files.cc
               src/messages/workspace_execute_command.cc
               src/messages/workspace_symbol.cc)
