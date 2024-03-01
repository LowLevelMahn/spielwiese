get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

set (HEADER_FILES
  "Compiler.hpp"
  "jitcee.h"
  "common.hpp"
  "Jit.hpp"
)

set (SOURCE_FILES
  "common.cpp"
  "Compiler.cpp"
  "Jit.cpp"
  "jitcee.cpp"
)

ADD_HEADER_FILTER( "${HEADER_FILES}" )
ADD_SOURCE_FILTER( "${SOURCE_FILES}" )

DEFINE_DYNAMIC_LIBRARY(${PROJECT_NAME} ".")

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

find_package(Clang REQUIRED CONFIG)

# Set your project compile flags.
# E.g. if using the C++ header files
# you will need to enable C++11 support
# for your compiler.

include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

# Link against LLVM libraries
target_link_libraries(${PROJECT_NAME} PRIVATE LLVMCore LLVMOrcJIT LLVMJITLink)

target_include_directories(${PROJECT_NAME} PRIVATE ${CLANG_INCLUDE_DIRS} )
target_link_libraries(${PROJECT_NAME} PRIVATE clangCodeGen LLVMX86Info LLVMX86CodeGen)
