add_executable(intro main.cpp)

target_link_libraries(
  intro
  PRIVATE my_lisp::my_lisp_options
          my_lisp::my_lisp_warnings
          my_lisp::lisp_library)

my_lisp_configure_linker(intro)

#target_include_directories(intro PUBLIC "${CMAKE_BINARY_DIR}/include")

# Apply WASM-specific configuration
if(EMSCRIPTEN)
  my_lisp_configure_wasm_target(intro
    TITLE "Simple LISP Interpreter"
    DESCRIPTION "My own little LISP implementation (incomplete)"
  )
endif()
