include(GenerateExportHeader)


add_library(lisp_library SymbolTable.cpp Checks.cpp Conversions.cpp)



add_library(my_lisp::lisp_library ALIAS lisp_library)

target_link_libraries(lisp_library PRIVATE my_lisp_options my_lisp_warnings)

target_include_directories(lisp_library ${WARNING_GUARD} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                                                                  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>)

if (my_lisp_BUILD_FUZZ_TESTS)
  target_link_libraries(lisp_library PRIVATE -fsanitize=fuzzer-no-link)
  target_compile_options(lisp_library PRIVATE -fsanitize=fuzzer-no-link)
endif()

target_compile_features(lisp_library PUBLIC cxx_std_${CMAKE_CXX_STANDARD})

my_lisp_configure_linker(lisp_library)

set_target_properties(
  lisp_library
  PROPERTIES VERSION ${PROJECT_VERSION}
             CXX_VISIBILITY_PRESET hidden
             VISIBILITY_INLINES_HIDDEN YES)

generate_export_header(lisp_library EXPORT_FILE_NAME ${PROJECT_BINARY_DIR}/include/my_lisp/lisp_library_export.hpp)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(lisp_library PUBLIC SAMPLE_LIBRARY_STATIC_DEFINE)
endif()
