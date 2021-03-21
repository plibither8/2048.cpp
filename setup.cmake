# run by:
# ctest -S setup.cmake

# --- Project-specific -Doptions
# these will be used if the project isn't already configured.
set(_opts)

# --- boilerplate follows

# CTEST_CMAKE_GENERATOR must always be defined
if(NOT DEFINED CTEST_CMAKE_GENERATOR AND DEFINED ENV{CMAKE_GENERATOR})
  set(CTEST_CMAKE_GENERATOR $ENV{CMAKE_GENERATOR})
endif()
if(NOT DEFINED CTEST_CMAKE_GENERATOR AND CMAKE_VERSION VERSION_GREATER_EQUAL 3.17)
  find_program(_gen NAMES ninja ninja-build samu)
  if(_gen)
    execute_process(COMMAND ${_gen} --version
      OUTPUT_VARIABLE _ninja_version
      OUTPUT_STRIP_TRAILING_WHITESPACE
      RESULT_VARIABLE _gen_ok
      TIMEOUT 10)
    if(_gen_ok EQUAL 0 AND _ninja_version VERSION_GREATER_EQUAL 1.10)
      set(CTEST_CMAKE_GENERATOR "Ninja")
    endif()
  endif(_gen)
endif()
if(NOT DEFINED CTEST_CMAKE_GENERATOR)
  set(CTEST_BUILD_FLAGS -j)  # not --parallel as this goes to generator directly
  if(WIN32)
    set(CTEST_CMAKE_GENERATOR "MinGW Makefiles")
  else()
    set(CTEST_CMAKE_GENERATOR "Unix Makefiles")
  endif()
endif()

if(NOT DEFINED CTEST_BUILD_CONFIGURATION)
  list(APPEND _opts -DCMAKE_BUILD_TYPE=Release)
endif()

set(CTEST_SOURCE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
if(NOT DEFINED CTEST_BINARY_DIRECTORY)
  set(CTEST_BINARY_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/build)
endif()

# -- build and test
ctest_start("Experimental")

ctest_configure(
  OPTIONS "${_opts}"
  RETURN_VALUE _ret
  CAPTURE_CMAKE_ERROR _err)
if(NOT (_ret EQUAL 0 AND _err EQUAL 0))
  message(FATAL_ERROR "Configure failed: return ${_ret} cmake return ${_err}")
endif()

ctest_build(
  RETURN_VALUE _ret
  CAPTURE_CMAKE_ERROR _err)
if(NOT (_ret EQUAL 0 AND _err EQUAL 0))
  message(FATAL_ERROR "Build failed.")
endif()

ctest_test(
  RETURN_VALUE _ret
  CAPTURE_CMAKE_ERROR _err)

if(NOT (_ret EQUAL 0 AND _err EQUAL 0))
  message(FATAL_ERROR "Test failed.")
endif()
