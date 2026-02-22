macro(my_lisp_configure_linker project_name)
  set(my_lisp_USER_LINKER_OPTION
    "DEFAULT"
      CACHE STRING "Linker to be used")
    set(my_lisp_USER_LINKER_OPTION_VALUES "DEFAULT" "SYSTEM" "LLD" "GOLD" "BFD" "MOLD" "SOLD" "APPLE_CLASSIC" "MSVC")
  set_property(CACHE my_lisp_USER_LINKER_OPTION PROPERTY STRINGS ${my_lisp_USER_LINKER_OPTION_VALUES})
  list(
    FIND
    my_lisp_USER_LINKER_OPTION_VALUES
    ${my_lisp_USER_LINKER_OPTION}
    my_lisp_USER_LINKER_OPTION_INDEX)

  if(${my_lisp_USER_LINKER_OPTION_INDEX} EQUAL -1)
    message(
      STATUS
        "Using custom linker: '${my_lisp_USER_LINKER_OPTION}', explicitly supported entries are ${my_lisp_USER_LINKER_OPTION_VALUES}")
  endif()

  set_target_properties(${project_name} PROPERTIES LINKER_TYPE "${my_lisp_USER_LINKER_OPTION}")
endmacro()
