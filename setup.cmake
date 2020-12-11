# run by:
# ctest -S setup.cmake

# --- Project-specific -Doptions
# these will be used if the project isn't already configured.
set(_opts)

# --- boilerplate follows

# CTEST_CMAKE_GENERATOR must always be defined
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
  set(CTEST_BUILD_CONFIGURATION "Release")
endif()

set(CTEST_SOURCE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
if(NOT DEFINED CTEST_BINARY_DIRECTORY)
  set(CTEST_BINARY_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/build)
endif()

# -- build and test
ctest_start("Experimental" ${CTEST_SOURCE_DIRECTORY} ${CTEST_BINARY_DIRECTORY})

ctest_configure(
  BUILD ${CTEST_BINARY_DIRECTORY}
  SOURCE ${CTEST_SOURCE_DIRECTORY}
  OPTIONS "${_opts}"
  RETURN_VALUE return_code
  CAPTURE_CMAKE_ERROR cmake_err)

# if it's a generator or compiler mismatch, delete cache and try again
if(NOT cmake_err EQUAL 0)
  file(REMOVE ${CTEST_BINARY_DIRECTORY}/CMakeCache.txt)

  ctest_configure(
    BUILD ${CTEST_BINARY_DIRECTORY}
    SOURCE ${CTEST_SOURCE_DIRECTORY}
    OPTIONS "${_opts}"
    RETURN_VALUE return_code
    CAPTURE_CMAKE_ERROR cmake_err)
endif()

if(return_code EQUAL 0 AND cmake_err EQUAL 0)
  ctest_build(
    BUILD ${CTEST_BINARY_DIRECTORY}
    CONFIGURATION ${CTEST_BUILD_CONFIGURATION}
    RETURN_VALUE return_code
    NUMBER_ERRORS Nerror
    CAPTURE_CMAKE_ERROR cmake_err
    )
else()
  message(STATUS "SKIP: ctest_build(): returncode: ${return_code}; CMake error code: ${cmake_err}")
endif()

if(return_code EQUAL 0 AND Nerror EQUAL 0 AND cmake_err EQUAL 0)
  ctest_test(
  BUILD ${CTEST_BINARY_DIRECTORY}
  RETURN_VALUE return_code
  CAPTURE_CMAKE_ERROR ctest_err
  PARALLEL_LEVEL ${Ncpu}
  )
else()
  message(STATUS "SKIP: ctest_test(): returncode: ${return_code}; CMake error code: ${cmake_err}")
endif()

if(NOT ((WIN32 OR return_code EQUAL 0) AND Nerror EQUAL 0 AND cmake_err EQUAL 0 AND (WIN32 OR ctest_err EQUAL 0)))
  message(FATAL_ERROR "Build and test failed.")
endif()
