get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

set (HEADER_FILES
  "common.hpp"
  "Compiler.hpp"
  "Jit.hpp"
  "Jitcee.hpp"
  "Dll_lib.hpp"
)

set (SOURCE_FILES
  "Compiler.cpp"
  "Jit.cpp"
  "Jitcee.cpp"
  "Dll_lib.cpp"
)

ADD_HEADER_FILTER( "${HEADER_FILES}" )
ADD_SOURCE_FILTER( "${SOURCE_FILES}" )

DEFINE_STATIC_LIBRARY(${PROJECT_NAME} ".")

