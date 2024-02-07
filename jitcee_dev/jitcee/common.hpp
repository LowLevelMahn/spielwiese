#pragma once

#include <iostream>
#include <source_location>
#include <sstream>
#include <stdexcept>

#include <llvm/Support/Error.h>

void throw_on_error(llvm::Error error, std::source_location location =
                                           std::source_location::current());

template<typename ValueT>
class Heaper
{
public:
    Heaper(ValueT value) : m_value(std::move(value)) {}
    const ValueT& data() { return m_value; }

  private:
    ValueT m_value;
};
