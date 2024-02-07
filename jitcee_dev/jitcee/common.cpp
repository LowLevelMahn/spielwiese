#include "common.hpp"

void throw_on_error(llvm::Error error_, const std::source_location location_ /*=
                                            std::source_location::current()*/) {
    if (error_) {
        std::stringstream sstr;
        sstr << "Error in file: " << location_.file_name()
             << " in function: " << location_.function_name()
             << " in line: " << location_.line()
             << " msg: " << llvm::toString(std::move(error_));
        std::cout << sstr.str() << "\n";
        throw std::runtime_error{sstr.str()};
    }
}
