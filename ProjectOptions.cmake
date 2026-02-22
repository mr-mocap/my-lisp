include(cmake/SystemLink.cmake)
include(cmake/LibFuzzer.cmake)
include(CMakeDependentOption)
include(CheckCXXCompilerFlag)


include(CheckCXXSourceCompiles)


macro(my_lisp_supports_sanitizers)
  # Emscripten doesn't support sanitizers
  if(EMSCRIPTEN)
    set(SUPPORTS_UBSAN OFF)
    set(SUPPORTS_ASAN OFF)
  elseif((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND NOT WIN32)

    message(STATUS "Sanity checking UndefinedBehaviorSanitizer, it should be supported on this platform")
    set(TEST_PROGRAM "int main() { return 0; }")

    # Check if UndefinedBehaviorSanitizer works at link time
    set(CMAKE_REQUIRED_FLAGS "-fsanitize=undefined")
    set(CMAKE_REQUIRED_LINK_OPTIONS "-fsanitize=undefined")
    check_cxx_source_compiles("${TEST_PROGRAM}" HAS_UBSAN_LINK_SUPPORT)

    if(HAS_UBSAN_LINK_SUPPORT)
      message(STATUS "UndefinedBehaviorSanitizer is supported at both compile and link time.")
      set(SUPPORTS_UBSAN ON)
    else()
      message(WARNING "UndefinedBehaviorSanitizer is NOT supported at link time.")
      set(SUPPORTS_UBSAN OFF)
    endif()
  else()
    set(SUPPORTS_UBSAN OFF)
  endif()

  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND WIN32)
    set(SUPPORTS_ASAN OFF)
  else()
    if (NOT WIN32)
      message(STATUS "Sanity checking AddressSanitizer, it should be supported on this platform")
      set(TEST_PROGRAM "int main() { return 0; }")

      # Check if AddressSanitizer works at link time
      set(CMAKE_REQUIRED_FLAGS "-fsanitize=address")
      set(CMAKE_REQUIRED_LINK_OPTIONS "-fsanitize=address")
      check_cxx_source_compiles("${TEST_PROGRAM}" HAS_ASAN_LINK_SUPPORT)

      if(HAS_ASAN_LINK_SUPPORT)
        message(STATUS "AddressSanitizer is supported at both compile and link time.")
        set(SUPPORTS_ASAN ON)
      else()
        message(WARNING "AddressSanitizer is NOT supported at link time.")
        set(SUPPORTS_ASAN OFF)
      endif()
    else()
      set(SUPPORTS_ASAN ON)
    endif()
  endif()
endmacro()

macro(my_lisp_setup_options)
  option(my_lisp_ENABLE_HARDENING "Enable hardening" ON)
  option(my_lisp_ENABLE_COVERAGE "Enable coverage reporting" OFF)
  cmake_dependent_option(
    my_lisp_ENABLE_GLOBAL_HARDENING
    "Attempt to push hardening options to built dependencies"
    ON
    my_lisp_ENABLE_HARDENING
    OFF)

  my_lisp_supports_sanitizers()

  if(NOT PROJECT_IS_TOP_LEVEL OR my_lisp_PACKAGING_MAINTAINER_MODE)
    option(my_lisp_ENABLE_IPO "Enable IPO/LTO" OFF)
    option(my_lisp_WARNINGS_AS_ERRORS "Treat Warnings As Errors" OFF)
    option(my_lisp_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" OFF)
    option(my_lisp_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(my_lisp_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" OFF)
    option(my_lisp_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(my_lisp_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(my_lisp_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(my_lisp_ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)
    option(my_lisp_ENABLE_CPPCHECK "Enable cpp-check analysis" OFF)
    option(my_lisp_ENABLE_PCH "Enable precompiled headers" OFF)
    option(my_lisp_ENABLE_CACHE "Enable ccache" OFF)
  else()
    option(my_lisp_ENABLE_IPO "Enable IPO/LTO" ON)
    option(my_lisp_WARNINGS_AS_ERRORS "Treat Warnings As Errors" ON)
    option(my_lisp_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" ${SUPPORTS_ASAN})
    option(my_lisp_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(my_lisp_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" ${SUPPORTS_UBSAN})
    option(my_lisp_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(my_lisp_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(my_lisp_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(my_lisp_ENABLE_CLANG_TIDY "Enable clang-tidy" ON)
    option(my_lisp_ENABLE_CPPCHECK "Enable cpp-check analysis" ON)
    option(my_lisp_ENABLE_PCH "Enable precompiled headers" OFF)
    option(my_lisp_ENABLE_CACHE "Enable ccache" ON)
  endif()

  if(NOT PROJECT_IS_TOP_LEVEL)
    mark_as_advanced(
      my_lisp_ENABLE_IPO
      my_lisp_WARNINGS_AS_ERRORS
      my_lisp_ENABLE_SANITIZER_ADDRESS
      my_lisp_ENABLE_SANITIZER_LEAK
      my_lisp_ENABLE_SANITIZER_UNDEFINED
      my_lisp_ENABLE_SANITIZER_THREAD
      my_lisp_ENABLE_SANITIZER_MEMORY
      my_lisp_ENABLE_UNITY_BUILD
      my_lisp_ENABLE_CLANG_TIDY
      my_lisp_ENABLE_CPPCHECK
      my_lisp_ENABLE_COVERAGE
      my_lisp_ENABLE_PCH
      my_lisp_ENABLE_CACHE)
  endif()

  my_lisp_check_libfuzzer_support(LIBFUZZER_SUPPORTED)
  if(LIBFUZZER_SUPPORTED AND (my_lisp_ENABLE_SANITIZER_ADDRESS OR my_lisp_ENABLE_SANITIZER_THREAD OR my_lisp_ENABLE_SANITIZER_UNDEFINED))
    set(DEFAULT_FUZZER ON)
  else()
    set(DEFAULT_FUZZER OFF)
  endif()

  option(my_lisp_BUILD_FUZZ_TESTS "Enable fuzz testing executable" ${DEFAULT_FUZZER})

endmacro()

macro(my_lisp_global_options)
  if(my_lisp_ENABLE_IPO)
    include(cmake/InterproceduralOptimization.cmake)
    my_lisp_enable_ipo()
  endif()

  my_lisp_supports_sanitizers()

  if(my_lisp_ENABLE_HARDENING AND my_lisp_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN 
       OR my_lisp_ENABLE_SANITIZER_UNDEFINED
       OR my_lisp_ENABLE_SANITIZER_ADDRESS
       OR my_lisp_ENABLE_SANITIZER_THREAD
       OR my_lisp_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    message("${my_lisp_ENABLE_HARDENING} ${ENABLE_UBSAN_MINIMAL_RUNTIME} ${my_lisp_ENABLE_SANITIZER_UNDEFINED}")
    my_lisp_enable_hardening(my_lisp_options ON ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()
endmacro()

macro(my_lisp_local_options)
  if(PROJECT_IS_TOP_LEVEL)
    include(cmake/StandardProjectSettings.cmake)
  endif()

  add_library(my_lisp_warnings INTERFACE)
  add_library(my_lisp_options INTERFACE)

  include(cmake/CompilerWarnings.cmake)
  my_lisp_set_project_warnings(
    my_lisp_warnings
    ${my_lisp_WARNINGS_AS_ERRORS}
    ""
    ""
    ""
    "")

  include(cmake/Linker.cmake)
  # Must configure each target with linker options, we're avoiding setting it globally for now

  if(NOT EMSCRIPTEN)
    include(cmake/Sanitizers.cmake)
    my_lisp_enable_sanitizers(
      my_lisp_options
      ${my_lisp_ENABLE_SANITIZER_ADDRESS}
      ${my_lisp_ENABLE_SANITIZER_LEAK}
      ${my_lisp_ENABLE_SANITIZER_UNDEFINED}
      ${my_lisp_ENABLE_SANITIZER_THREAD}
      ${my_lisp_ENABLE_SANITIZER_MEMORY})
  endif()

  set_target_properties(my_lisp_options PROPERTIES UNITY_BUILD ${my_lisp_ENABLE_UNITY_BUILD})

  if(my_lisp_ENABLE_PCH)
    target_precompile_headers(
      my_lisp_options
      INTERFACE
      <vector>
      <string>
      <utility>)
  endif()

  if(my_lisp_ENABLE_CACHE)
    include(cmake/Cache.cmake)
    my_lisp_enable_cache()
  endif()

  include(cmake/StaticAnalyzers.cmake)
  if(my_lisp_ENABLE_CLANG_TIDY)
    my_lisp_enable_clang_tidy(my_lisp_options ${my_lisp_WARNINGS_AS_ERRORS})
  endif()

  if(my_lisp_ENABLE_CPPCHECK)
    my_lisp_enable_cppcheck(${my_lisp_WARNINGS_AS_ERRORS} "" # override cppcheck options
    )
  endif()

  if(my_lisp_ENABLE_COVERAGE)
    include(cmake/Tests.cmake)
    my_lisp_enable_coverage(my_lisp_options)
  endif()

  if(my_lisp_WARNINGS_AS_ERRORS)
    check_cxx_compiler_flag("-Wl,--fatal-warnings" LINKER_FATAL_WARNINGS)
    if(LINKER_FATAL_WARNINGS)
      # This is not working consistently, so disabling for now
      # target_link_options(my_lisp_options INTERFACE -Wl,--fatal-warnings)
    endif()
  endif()

  if(my_lisp_ENABLE_HARDENING AND NOT my_lisp_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN 
       OR my_lisp_ENABLE_SANITIZER_UNDEFINED
       OR my_lisp_ENABLE_SANITIZER_ADDRESS
       OR my_lisp_ENABLE_SANITIZER_THREAD
       OR my_lisp_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    my_lisp_enable_hardening(my_lisp_options OFF ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()

endmacro()
