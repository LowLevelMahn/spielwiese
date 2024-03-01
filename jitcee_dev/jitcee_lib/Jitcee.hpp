#pragma once

#include "Dll_lib.hpp"

#include "Compiler.hpp"
#include "Jit.hpp"

#include <filesystem>

namespace jitcee
{
    class Jitcee
    {
        public:
            explicit Jitcee(const std::filesystem::path& path_to_dll_ );
            ~Jitcee();

            Compiler create_compiler();
            Jit create_jit();

        private:
            Jitcee(Jitcee const&) = delete;
            Jitcee& operator=(Jitcee const&) = delete;

            Dll_lib m_dll_lib;
            f_void* m_jitcee_initialize{};
            f_void* m_jitcee_shutdown{};
    };
}
