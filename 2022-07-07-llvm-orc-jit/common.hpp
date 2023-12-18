#pragma once

#include <iostream>
#include <source_location>
#include <sstream>
#include <stdexcept>

#include <llvm/Support/Error.h>

void throw_on_error(llvm::Error error, std::source_location location =
                                           std::source_location::current());
