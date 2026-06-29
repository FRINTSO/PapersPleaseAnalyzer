function(set_project_warnings project_name)

  set(MSVC_WARNINGS
    /W4 # Baseline reasonable warnings
    /permissive- # standards conformance mode for MSVC compiler
  )

  set(CLANG_WARNINGS
    -Wall
    -Wundef # warn on undefined macros in #if (silent 0 otherwise)
    -Wno-trigraphs
    -Wno-sign-compare
    -Wvla # warn on variable-length arrays (stack bombs)
    -Wnull-dereference # warn if a null dereference is detected
    -Wframe-larger-than=2048 # catch accidental huge stack allocations
    -Werror=return-type # missing return is always a bug
    -fno-strict-aliasing # don't trust strict aliasing optimizations
  )

  set(GCC_WARNINGS
    ${CLANG_WARNINGS}
    -Wmisleading-indentation # warn if indentation implies blocks where blocks
                             # do not exist (goto-fail style bugs)
    -Wduplicated-cond # warn if if / else chain has duplicated conditions
    -Wlogical-op # warn about logical operations being used where bitwise were
                 # probably intended
  )

  if (MSVC)
    set(PROJECT_WARNINGS ${MSVC_WARNINGS})
  elseif (CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    set(PROJECT_WARNINGS ${CLANG_WARNINGS})
  elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(PROJECT_WARNINGS ${GCC_WARNINGS})
  else ()
    message(AUTHOR_WARNING "No compiler warnings set for '${CMAKE_CXX_COMPILER_ID}' compiler.")
  endif ()

  target_compile_options(${project_name} INTERFACE ${PROJECT_WARNINGS})

endfunction()
