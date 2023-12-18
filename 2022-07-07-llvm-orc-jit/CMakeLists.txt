cmake_minimum_required(VERSION 3.20.0)
project(llvm-orc-jit)

set (CMAKE_CXX_STANDARD 20)

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

# Now build our tools
add_executable(llvm-orc-jit main.cc common.cpp jit.h ccompiler.h common.hpp)

# Find the libraries that correspond to the LLVM components
# that we wish to use
#llvm_map_components_to_libnames(llvm_libs support core orcjit jitlink)
# Link against LLVM libraries
#target_link_libraries(llvm-orc-jit ${llvm_libs})

# Link against LLVM libraries
target_link_libraries(llvm-orc-jit LLVMSupport LLVMCore LLVMOrcJIT LLVMJITLink)

target_include_directories(llvm-orc-jit PRIVATE ${CLANG_INCLUDE_DIRS} )
target_link_libraries(llvm-orc-jit clangTooling clangCodeGen clangSupport LLVMX86Info LLVMX86TargetMCA LLVMX86CodeGen)